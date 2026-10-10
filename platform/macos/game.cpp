/* Drives the existing game loop on the SDL OpenGL window. */
#include "si_Main.h"
#include "si_Loading.h"

#include <SDL.h>

#include <cstdio>
#include <cstring>

extern GL_Window window;
extern bool game_loading;
extern unsigned long h_thread2;

static Application g_application;
static Keys g_keys;

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

int macos_game_main(SDL_Window* sdl_window, int selftest)
{
	prepare_window();

	if (Initialize(&window, &g_keys) == FALSE)
	{
		std::fprintf(stderr, "Initialize failed\n");
		return 1;
	}

	int frames = 0;
	bool running = true;
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

		SDL_GL_SwapWindow(sdl_window);
		frames++;
		if (selftest && frames >= 8)
			break;
	}

	std::printf("game_step: %d\n", game_step);
	std::fflush(stdout);
	Deinitialize();
	return 0;
}
