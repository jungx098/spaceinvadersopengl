/* FMOD 3 calls used by the game, backed by SDL_mixer. */
#include "si_fmod.h"

#include <SDL_mixer.h>

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

signed char FSOUND_Init(int mixrate, int maxchannels, unsigned int flags)
{
	int rate = mixrate > 0 ? mixrate : 44100;
	int channels = maxchannels > 0 ? maxchannels : 32;
	(void)flags;
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_MOD);
	if (Mix_OpenAudio(rate, AUDIO_S16SYS, 2, 1024) < 0)
		return 0;
	Mix_AllocateChannels(channels);
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
