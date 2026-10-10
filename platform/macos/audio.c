/* FMOD 3 calls used by the game, backed by SDL_mixer. */
#include "si_fmod.h"

#include <SDL_mixer.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sample {
	Mix_Chunk* chunk;
} Sample;

typedef struct Module {
	Mix_Music* music;
	int playing;
} Module;

static int audio_ready = 0;
static int g_rate = 44100;
static float g_spectrum[512];
static float g_pcm[512];
static int g_pcm_write = 0;

static void fft_512(float* re, float* im)
{
	int n = 512;
	int i;
	for (i = 1, n = 0; i < 512; i++)
	{
		int bit = 256;
		for (; n & bit; bit >>= 1)
			n ^= bit;
		n ^= bit;
		if (i < n)
		{
			float tr = re[i];
			float ti = im[i];
			re[i] = re[n];
			im[i] = im[n];
			re[n] = tr;
			im[n] = ti;
		}
	}
	for (n = 2; n <= 512; n <<= 1)
	{
		float ang = -2.0f * 3.14159265f / (float)n;
		float wlen_r = cosf(ang);
		float wlen_i = sinf(ang);
		for (i = 0; i < 512; i += n)
		{
			float wr = 1.0f;
			float wi = 0.0f;
			int j;
			for (j = 0; j < n / 2; j++)
			{
				int u = i + j;
				int v = u + n / 2;
				float vr = re[v] * wr - im[v] * wi;
				float vi = re[v] * wi + im[v] * wr;
				float nwr = wr * wlen_r - wi * wlen_i;
				re[v] = re[u] - vr;
				im[v] = im[u] - vi;
				re[u] += vr;
				im[u] += vi;
				wi = wr * wlen_i + wi * wlen_r;
				wr = nwr;
			}
		}
	}
}

static void postmix(void* udata, Uint8* stream, int len)
{
	Sint16* in = (Sint16*)stream;
	int frames = len / (int)(sizeof(Sint16) * 2);
	float re[512];
	float im[512];
	int i;
	int start;
	(void)udata;
	for (i = 0; i < frames; i++)
	{
		g_pcm[g_pcm_write & 511] = (in[i * 2] + in[i * 2 + 1]) * (1.0f / 65536.0f);
		g_pcm_write++;
	}
	start = g_pcm_write - 512;
	for (i = 0; i < 512; i++)
	{
		float window = 0.5f * (1.0f - cosf(2.0f * 3.14159265f * (float)i / 512.0f));
		int idx = start + i;
		re[i] = (idx >= 0 ? g_pcm[idx & 511] : 0.0f) * window;
		im[i] = 0.0f;
	}
	fft_512(re, im);
	{
		/* The equalizer reads bins 0..159 as eight groups of 20 and divides
		   each group by 4. Pack a log-frequency spectrum into those bins so
		   each bar is one musical band instead of a 1.7 kHz slice of bass. */
		static const float edge_hz[] = {60, 150, 300, 600, 1000, 1800, 3200, 5500, 9000};
		float mag[512];
		int band;
		memset(g_spectrum, 0, sizeof g_spectrum);
		for (i = 0; i < 512; i++)
			mag[i] = sqrtf(re[i] * re[i] + im[i] * im[i]);
		for (band = 0; band < 8; band++)
		{
			int lo = (int)(edge_hz[band] * 512.0f / (float)g_rate);
			int hi = (int)(edge_hz[band + 1] * 512.0f / (float)g_rate);
			float sum = 0.0f;
			float level;
			int k;
			if (lo < 1)
				lo = 1;
			if (hi <= lo)
				hi = lo + 1;
			if (hi > 256)
				hi = 256;
			for (k = lo; k < hi; k++)
				sum += mag[k];
			sum /= (float)(hi - lo);
			{
				static const float tilt[] = {1.0f, 1.1f, 1.2f, 2.0f, 3.5f, 5.0f, 7.0f, 9.0f};
				sum *= tilt[band];
			}
			/* A full bar is an average bin of 0.2, because 20 * 0.2 / 4 = 1. */
			level = sum / 25.0f;
			if (level > 1.0f)
				level = 1.0f;
			for (k = 0; k < 20; k++)
				g_spectrum[band * 20 + k] = 0.2f * level;
		}
	}
}

signed char FSOUND_Init(int mixrate, int maxchannels, unsigned int flags)
{
	int rate = mixrate > 0 ? mixrate : 44100;
	int channels = maxchannels > 0 ? maxchannels : 32;
	(void)flags;
	g_rate = rate;
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_MOD);
	if (Mix_OpenAudio(rate, AUDIO_S16SYS, 2, 1024) < 0)
		return 0;
	Mix_AllocateChannels(channels);
	Mix_SetPostMix(postmix, 0);
	audio_ready = 1;
	return 1;
}

void FSOUND_Close(void)
{
	if (!audio_ready)
		return;
	Mix_HaltChannel(-1);
	Mix_HaltMusic();
	Mix_CloseAudio();
	Mix_Quit();
	audio_ready = 0;
}

FSOUND_SAMPLE* FSOUND_Sample_Load(int index, const char* name_or_data, unsigned int mode, int offset, int length)
{
	SDL_RWops* rw;
	Mix_Chunk* chunk;
	Sample* sample;
	(void)index;
	if (!audio_ready || !(mode & FSOUND_LOADMEMORY) || !name_or_data || length <= 0)
		return 0;
	rw = SDL_RWFromConstMem(name_or_data + offset, length);
	if (!rw)
		return 0;
	chunk = Mix_LoadWAV_RW(rw, 1);
	if (!chunk)
		return 0;
	sample = (Sample*)calloc(1, sizeof(Sample));
	if (!sample)
	{
		Mix_FreeChunk(chunk);
		return 0;
	}
	sample->chunk = chunk;
	{
		static int announced = 0;
		if (!announced)
		{
			announced = 1;
			printf("sample_loaded: 1\n");
			fflush(stdout);
		}
	}
	return (FSOUND_SAMPLE*)sample;
}

int FSOUND_PlaySoundEx(int channel, FSOUND_SAMPLE* sptr, FSOUND_DSPUNIT* dsp, signed char startpaused)
{
	Sample* sample = (Sample*)sptr;
	int played;
	(void)dsp;
	if (!audio_ready || !sample || !sample->chunk || startpaused)
		return -1;
	played = Mix_PlayChannel(channel < 0 ? -1 : channel, sample->chunk, 0);
	return played;
}

signed char FSOUND_SetVolume(int channel, int volume)
{
	int scaled = volume / 2;
	if (scaled < 0)
		scaled = 0;
	if (scaled > MIX_MAX_VOLUME)
		scaled = MIX_MAX_VOLUME;
	if (channel < 0)
		Mix_Volume(-1, scaled);
	else
		Mix_Volume(channel, scaled);
	return 1;
}

signed char FSOUND_IsPlaying(int channel)
{
	return Mix_Playing(channel) ? 1 : 0;
}

signed char FSOUND_SetCurrentPosition(int channel, unsigned int offset)
{
	(void)channel;
	(void)offset;
	return 1;
}

float FSOUND_GetCPUUsage(void)
{
	return 0.0f;
}

float* FSOUND_DSP_GetSpectrum(void)
{
	return g_spectrum;
}

FSOUND_DSPUNIT* FSOUND_DSP_GetFFTUnit(void)
{
	static int unit = 1;
	return (FSOUND_DSPUNIT*)&unit;
}

FMUSIC_MODULE* FMUSIC_LoadSongEx(const char* name_or_data, int offset, int length, unsigned int mode, const int* samplelist, int samplelistnum)
{
	SDL_RWops* rw;
	Mix_Music* music;
	Module* module;
	(void)samplelist;
	(void)samplelistnum;
	if (!audio_ready || !(mode & FSOUND_LOADMEMORY) || !name_or_data || length <= 0)
		return 0;
	rw = SDL_RWFromConstMem(name_or_data + offset, length);
	if (!rw)
		return 0;
	music = Mix_LoadMUS_RW(rw, 1);
	if (!music)
	{
		fprintf(stderr, "music load failed: %s\n", Mix_GetError());
		return 0;
	}
	module = (Module*)calloc(1, sizeof(Module));
	if (!module)
	{
		Mix_FreeMusic(music);
		return 0;
	}
	module->music = music;
	return (FMUSIC_MODULE*)module;
}

signed char FMUSIC_FreeSong(FMUSIC_MODULE* mod)
{
	Module* module = (Module*)mod;
	if (!module)
		return 0;
	if (module->music)
		Mix_FreeMusic(module->music);
	free(module);
	return 1;
}

signed char FMUSIC_PlaySong(FMUSIC_MODULE* mod)
{
	Module* module = (Module*)mod;
	if (!audio_ready || !module || !module->music)
		return 0;
	if (Mix_PlayMusic(module->music, -1) != 0)
		return 0;
	module->playing = 1;
	printf("music_playing: %d\n", Mix_PlayingMusic());
	fflush(stdout);
	return 1;
}

signed char FMUSIC_StopSong(FMUSIC_MODULE* mod)
{
	Module* module = (Module*)mod;
	Mix_HaltMusic();
	if (module)
		module->playing = 0;
	return 1;
}

signed char FMUSIC_SetMasterVolume(FMUSIC_MODULE* mod, int volume)
{
	int scaled = volume / 2;
	(void)mod;
	if (scaled < 0)
		scaled = 0;
	if (scaled > MIX_MAX_VOLUME)
		scaled = MIX_MAX_VOLUME;
	Mix_VolumeMusic(scaled);
	return 1;
}

signed char FMUSIC_IsPlaying(FMUSIC_MODULE* mod)
{
	(void)mod;
	return Mix_PlayingMusic() ? 1 : 0;
}

signed char FMUSIC_IsFinished(FMUSIC_MODULE* mod)
{
	(void)mod;
	return Mix_PlayingMusic() ? 0 : 1;
}
