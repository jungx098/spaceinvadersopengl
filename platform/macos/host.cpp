// macOS host for checklist item 1: a window and a legacy OpenGL context.
// Later items call the existing game loop from here.

#include <SDL.h>
#include <OpenGL/gl.h>

#include <cstdio>
#include <cstdlib>

int macos_game_main(SDL_Window* window, SDL_GLContext context, int selftest);

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
	int code = macos_game_main(window, context, selftest ? 1 : 0);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return code;
}
