/* Drives the existing game loop on the SDL OpenGL window. */
#include "si_Main.h"
#include "si_Loading.h"
#include "si_Textures.h"
#include "macos_port.h"

#include <SDL.h>

#include <cstdio>
#include <cstring>

extern GL_Window window;
extern bool game_loading;
extern unsigned long h_thread2;

static Application g_application;
static Keys g_keys;
static SDL_Window* g_sdl_window = 0;
static SDL_GLContext g_sdl_context = 0;
static __thread int g_gl_current = 0;

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

	int frames = 0;
	bool running = true;
	Uint32 started = SDL_GetTicks();
	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
				running = false;
		}

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
			break;
		default:
			running = false;
			break;
		}

		if (game_step != 2)
			SDL_GL_SwapWindow(sdl_window);
		frames++;
		if (selftest && (game_step >= 4 || SDL_GetTicks() - started > 20000))
			break;
	}

	std::printf("game_step: %d\n", game_step);
	std::fflush(stdout);
	Deinitialize();
	return 0;
}
