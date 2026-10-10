/* Drives the existing game loop on the SDL OpenGL window. */
#include "si_Main.h"
#include "si_Loading.h"
#include "si_Textures.h"
#include "si_3DS.h"
#include "macos_port.h"

#include <SDL.h>

#include <cstdio>
#include <cstring>

extern GL_Window window;
extern bool game_loading;
extern unsigned long h_thread2;

static Application g_application;
static Keys g_keys;
extern bool moving;
extern double startx, starty, angle_x, angle_y;
extern Model_3DS m;
extern Model_3DS m1;

static int vk_from_sdl(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_LEFT: return VK_LEFT;
	case SDLK_RIGHT: return VK_RIGHT;
	case SDLK_UP: return VK_UP;
	case SDLK_ESCAPE: return VK_ESCAPE;
	case SDLK_LCTRL:
	case SDLK_RCTRL: return VK_CONTROL;
	case SDLK_SPACE: return VK_SPACE;
	case SDLK_RETURN:
	case SDLK_KP_ENTER: return VK_RETURN;
	case SDLK_KP_PLUS:
	case SDLK_EQUALS: return VK_ADD;
	case SDLK_KP_MINUS:
	case SDLK_MINUS: return VK_SUBTRACT;
	case SDLK_F1: return VK_F1;
	case SDLK_F2: return VK_F2;
	case SDLK_F3: return VK_F3;
	case SDLK_F4: return VK_F4;
	case SDLK_F5: return VK_F5;
	case SDLK_F6: return VK_F6;
	case SDLK_F10: return VK_F10;
	case SDLK_F12: return VK_F12;
	default:
		if (key >= SDLK_a && key <= SDLK_z)
			return (int)(key - SDLK_a + 'A');
		if (key >= 'A' && key <= 'Z')
			return (int)key;
		return -1;
	}
}

static void set_key(SDL_Keycode key, BOOL down)
{
	int vk = vk_from_sdl(key);
	if (vk >= 0 && vk < 256)
		g_keys.keyDown[vk] = down;
}
static SDL_Window* g_sdl_window = 0;
static SDL_GLContext g_sdl_context = 0;
static __thread int g_gl_current = 0;
static const int k_windowed_width = 640;
static const int k_windowed_height = 480;

static void apply_window_size(int width, int height)
{
	if (width < 1)
		width = 1;
	if (height < 1)
		height = 1;
	window.init.width = width;
	window.init.height = height;
}

static void toggle_fullscreen(void)
{
	if (!g_sdl_window)
		return;
	Uint32 flags = SDL_GetWindowFlags(g_sdl_window);
	if (flags & SDL_WINDOW_FULLSCREEN)
	{
		SDL_SetWindowFullscreen(g_sdl_window, 0);
		SDL_SetWindowSize(g_sdl_window, k_windowed_width, k_windowed_height);
		SDL_SetWindowPosition(g_sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		window.init.isFullScreen = FALSE;
		apply_window_size(k_windowed_width, k_windowed_height);
		return;
	}

	SDL_SetWindowFullscreen(g_sdl_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	window.init.isFullScreen = TRUE;
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(g_sdl_window, &width, &height);
	if (width > 0 && height > 0)
		apply_window_size(width, height);
}

extern "C" BOOL wglMakeCurrent(HDC hdc, HGLRC rc)
{
	if (!g_sdl_window)
		return FALSE;
	if (!hdc || !rc)
	{
		g_gl_current = 0;
		return SDL_GL_MakeCurrent(g_sdl_window, NULL) == 0 ? TRUE : FALSE;
	}
	g_gl_current = SDL_GL_MakeCurrent(g_sdl_window, g_sdl_context) == 0;
	return g_gl_current ? TRUE : FALSE;
}

extern "C" HGLRC wglGetCurrentContext(void)
{
	return g_gl_current ? (HGLRC)1 : 0;
}

static BOOL macos_swap_interval(int interval)
{
	return SDL_GL_SetSwapInterval(interval) == 0 ? TRUE : FALSE;
}

extern "C" PROC wglGetProcAddress(LPCSTR name)
{
	if (name && std::strcmp(name, "wglSwapIntervalEXT") == 0)
		return (PROC)macos_swap_interval;
	return 0;
}

extern "C" BOOL SwapBuffers(HDC hdc)
{
	(void)hdc;
	if (!g_sdl_window)
		return FALSE;
	SDL_GL_SwapWindow(g_sdl_window);
	return TRUE;
}

static void prepare_window(void)
{
	std::memset(&window, 0, sizeof window);
	std::memset(&g_keys, 0, sizeof g_keys);
	std::memset(&g_application, 0, sizeof g_application);

	g_application.className = "OpenGL";
	g_application.hInstance = (HINSTANCE)1;

	window.keys = &g_keys;
	window.init.application = &g_application;
	window.init.title = (char*)"Space Invaders OpenGL";
	window.init.width = 640;
	window.init.height = 480;
	window.init.bitsPerPixel = 32;
	window.init.isFullScreen = FALSE;
	window.init.quickFullScreen = FALSE;
	window.hWnd = (HWND)1;
	window.hDC = (HDC)1;
	window.hRC = (HGLRC)1;
	window.isVisible = TRUE;
}

int macos_game_main(SDL_Window* sdl_window, SDL_GLContext context, int selftest)
{
	g_sdl_window = sdl_window;
	g_sdl_context = context;
	g_gl_current = 1;
	if (selftest)
	{
		GLint samples = 0;
		typedef BOOL (*SwapIntervalFn)(int);
		SwapIntervalFn swap_interval;
		glGetIntegerv(GL_SAMPLES, &samples);
		std::printf("aa_samples: %d\n", samples);
		swap_interval = (SwapIntervalFn)wglGetProcAddress("wglSwapIntervalEXT");
		if (swap_interval)
			swap_interval(1);
		std::printf("vsync: %d\n", SDL_GL_GetSwapInterval());
		std::fflush(stdout);
	}
	prepare_window();

	if (selftest)
	{
		std::printf("asset_ship_bmp: %u\n", macos_asset_bytes(IDB_01, "BITMAP"));
		std::printf("asset_player_3ds: %u\n", macos_asset_bytes(IDR_BINARY_PLAYER_3DS, "BINARY"));
		std::printf("asset_xm: %u\n", macos_asset_bytes(IDR_SOUND_XM, "WAVE"));
		std::fflush(stdout);
	}

	if (Initialize(&window, &g_keys) == FALSE)
	{
		std::fprintf(stderr, "Initialize failed\n");
		return 1;
	}

	if (selftest)
	{
		BOOL ship = LoadTexture(NULL, IDB_01, 1);
		BOOL tga = LoadTexture(NULL, IDT_00, 2);
		BOOL jpg = LoadTexture(NULL, IDJ_09, 3);
		BOOL png = LoadTexture(NULL, IDP_35, 4);
		std::printf("load_texture_ship: %d\n", ship ? 1 : 0);
		std::printf("load_texture_tga: %d\n", tga ? 1 : 0);
		std::printf("load_texture_jpg: %d\n", jpg ? 1 : 0);
		std::printf("load_texture_png: %d\n", png ? 1 : 0);
		SDL_GL_SwapWindow(sdl_window);
		std::printf("swap_after_load: 1\n");
		std::fflush(stdout);
	}

	Fps_Init();

	if (selftest)
	{
		SDL_Event injected;
		std::memset(&injected, 0, sizeof injected);
		injected.type = SDL_KEYDOWN;
		injected.key.state = SDL_PRESSED;
		injected.key.keysym.sym = SDLK_LEFT;
		SDL_PushEvent(&injected);
	}

	int frames = 0;
	int play_frames = 0;
	bool running = true;
	Uint32 started = SDL_GetTicks();
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_F12 && !event.key.repeat && !selftest)
					toggle_fullscreen();
				set_key(event.key.keysym.sym, TRUE);
			}
			else if (event.type == SDL_KEYUP)
				set_key(event.key.keysym.sym, FALSE);
			else if (event.type == SDL_WINDOWEVENT &&
				(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
				 event.window.event == SDL_WINDOWEVENT_RESIZED))
			{
				apply_window_size(event.window.data1, event.window.data2);
				Uint32 flags = SDL_GetWindowFlags(g_sdl_window);
				window.init.isFullScreen = (flags & SDL_WINDOW_FULLSCREEN) ? TRUE : FALSE;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && game_step == 4)
			{
				moving = true;
				startx = event.button.x;
				starty = event.button.y;
			}
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				moving = false;
			else if (event.type == SDL_MOUSEMOTION && game_step == 4 && moving)
			{
				angle_x += (event.motion.x - startx) / 2.0;
				angle_y += (event.motion.y - starty) / 2.0;
				startx = event.motion.x;
				starty = event.motion.y;
			}
		}
		if (g_keys.keyDown[VK_ESCAPE])
			running = false;

		switch (game_step)
		{
		case 1:
			loading_display1();
			Update(0, &window);
			break;
		case 2:
			if (!game_loading)
			{
				wglMakeCurrent(NULL, NULL);
				h_thread2 = _beginthread(loading, 0, NULL);
				game_loading = true;
			}
			SDL_Delay(5);
			Update(0, &window);
			break;
		case 3:
			loading_display3();
			Update(0, &window);
			break;
		case 4:
			Fps_Update();
			Update(0, &window);
			GameLogic(&window);
			Draw(&window);
			play_frames++;
			break;
		default:
			running = false;
			break;
		}

		if (game_step != 2)
			SDL_GL_SwapWindow(sdl_window);
		frames++;
		if (selftest && play_frames >= 3)
		{
			int lit = 0;
			unsigned char sample[4];
			for (int y = 40; y < 440; y += 20)
			{
				for (int x = 40; x < 600; x += 20)
				{
					glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, sample);
					if (sample[0] || sample[1] || sample[2])
						lit++;
				}
			}
			std::printf("playfield_samples: %d\n", lit);
			std::printf("model_player: %d\n", m.numObjects);
			std::printf("model_ships: %d\n", m1.numObjects);
			{
				float* spec = FSOUND_DSP_GetSpectrum();
				int band;
				std::printf("equalizer:");
				for (band = 0; band < 8; band++)
					std::printf(" %.2f", spec ? spec[band * 20] / 0.2f : 0.0f);
				std::printf("\n");
			}
			{
				float* spec = FSOUND_DSP_GetSpectrum();
				float peak = 0.0f;
				int bin;
				if (spec)
				{
					for (bin = 0; bin < 160; bin++)
					{
						if (spec[bin] > peak)
							peak = spec[bin];
					}
				}
				std::printf("spectrum_peak: %f\n", peak);
			}
			break;
		}
		if (selftest && SDL_GetTicks() - started > 20000)
			break;
	}

	std::printf("game_step: %d\n", game_step);
	std::printf("input_left: %d\n", g_keys.keyDown[VK_LEFT] ? 1 : 0);
	if (selftest)
	{
		HKEY key = 0;
		DWORD disposition = 0;
		DWORD stored = 77;
		DWORD loaded = 0;
		DWORD type = 0;
		DWORD size = sizeof loaded;
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, (LPSTR)"", 0, 0, 0, &key, &disposition);
		RegSetValueEx(key, "port_selftest", 0, REG_DWORD, (const BYTE*)&stored, sizeof stored);
		RegCloseKey(key);
		key = 0;
		loaded = 0;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\SpaceInvaders-OpenGL\\", 0, KEY_READ, &key);
		RegQueryValueEx(key, "port_selftest", 0, &type, (LPBYTE)&loaded, &size);
		RegCloseKey(key);
		std::printf("save_roundtrip: %u\n", loaded);
	}
	std::fflush(stdout);
	Deinitialize();
	return 0;
}
