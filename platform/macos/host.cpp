// macOS host for checklist item 1: a window and a legacy OpenGL context.
// Later items call the existing game loop from here.

#include <SDL.h>
#include <OpenGL/gl.h>

#include <cstdio>
#include <cstdlib>

static void fail(const char* what)
{
	std::fprintf(stderr, "%s: %s\n", what, SDL_GetError());
	SDL_Quit();
	std::exit(1);
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		fail("SDL_Init");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow(
		"Space Invaders OpenGL",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)
		fail("SDL_CreateWindow");

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!context)
		fail("SDL_GL_CreateContext");

	const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
	const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	std::printf("gl_version: %s\n", version ? version : "(null)");
	std::printf("gl_renderer: %s\n", renderer ? renderer : "(null)");
	std::fflush(stdout);

	const bool selftest = std::getenv("SPACEINVADERS_SELFTEST") != nullptr;
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

		glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);

		if (selftest)
			break;
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
