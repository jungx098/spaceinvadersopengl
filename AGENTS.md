# AGENTS.md

Space Invaders OpenGL 0.6.1 (May 2004), a GPL-2.0-or-later Win32 clone of the arcade game. Fixed-function OpenGL, a Win32 message loop, and FMOD audio. Gameplay state is global C data. Match that style.

Human-facing overview, controls, and version history are in `Readme.txt` (also `Readme_fr.txt`, `Readme_es.txt`).

## Build

Open `Space Invaders.sln`. Configurations are `Debug|Win32` and `Release|Win32` only: x86, `MultiByte`, subsystem Windows.

| Config | Output |
| --- | --- |
| Debug | `Space Invaders OpenGL.exe` in the repo root |
| Release | `Release\Space Invaders OpenGL.exe` |

Vendored link inputs in the repo root: `fmodvc.lib`, `glpng.lib`, and delay-loaded `fmod.dll`. System libs include `opengl32`, `glu32`, `winmm`, `wininet`, `comctl32`, `wsock32`, `msacm32`. Preprocessor defines include `WIN32`, `_WINDOWS`, `_WIN32_WINDOWS=0x0410`, and `CONFIG_FMOD_EXTRACT_YES` (resource compile uses `FMOD_EXTRACT_YES`).

`Space Invaders.dsp` / `.dsw` are the VC6 projects. `Space Invaders.vcproj` is the intermediate upgrade. Change `Space Invaders.vcxproj` (and `.filters` when adding files) unless the task is specifically about an older project file.

Windows still builds only from `Space Invaders.sln`. There is no Makefile or CI. Do not replace that solution.

macOS builds from `CMakeLists.txt`. See `docs/macos-port.md`. Homebrew SDL2 and SDL2_mixer supply the window, input, and audio. The game draws with an OpenGL 2.1 compatibility context. `./build/spaceinvaders` is the game. F12 toggles fullscreen. `SPACEINVADERS_SELFTEST=1 ./build/spaceinvaders` runs until three gameplay frames, or 20 seconds, and prints the checks in that note. When a Windows MSVC toolchain is available, also build `Debug|Win32`. When it is not, say the Windows build is unverified.

## Where to change things

| Area | Files |
| --- | --- |
| `WinMain`, window, pixel format, message pump | `si_Basecode.cpp`, `si_Basecode.h` |
| Globals, input, `Update`, `Draw`, `GameLogic` | `si_Main.cpp`, `si_Main.h` |
| Ships, player, bunkers, HUD, background, console | `si_World.cpp`, `si_World.h` |
| Loading thread, sounds, texture load list | `si_Loading.cpp`, `si_Loading.h` |
| Explosions and 3D shot particles | `si_Particles.cpp`, `si_Particles.h` |
| BMP/TGA/PNG/JPG/GIF loading | `si_Textures.cpp`, `si_Textures.h` |
| 3DS models (`Model_3DS`) | `si_3DS.cpp`, `si_3DS.h`, `si_3DS_MemoryFile.*`, `si_3DS_Texture.*` |
| Bitmap font (C, GLTexFont) | `si_Font.c`, `si_Font.h` |
| Collisions, Bézier camera, hiscore, options, errors, UI translation | `si_Functions.cpp`, `si_Functions.h` |
| Frame timing | `si_Fps.cpp`, `si_Fps.h` |
| Extract embedded binaries at runtime | `si_Resources.cpp` |
| Embedded assets and dialogs | `resource.rc`, `resource.h` |
| Loose assets the resource script points at | `images\`, `sounds\`, `models\` |
| macOS window, input, fullscreen, audio, saves | `platform/macos/`, `CMakeLists.txt`, `docs/macos-port.md` |

`Update`, `Draw`, and `GameLogic` are large. Edit the existing function. Do not split these files unless asked.

## Runtime

`game_step` in `si_Main.cpp` selects the frame in `WinMain`:

1. Loading fade-in (`loading_display1`, then `Update`).
2. Background load. `loading` runs on a CRT thread from `_beginthread`. The main thread `Sleep(5)` and does not `SwapBuffers` (the loader swaps its own buffers).
3. Loading fade-out (`loading_display3`).
4. Play: `Fps_Update`, `Update`, `GameLogic`, `Draw`, then `SwapBuffers`.

Movement uses frame time from `si_Fps` (`ms`, `ms10`). Keep gameplay speeds independent of frame rate.

Keyboard state is `Keys::keyDown[256]` on `GL_Window`. Gameplay keys are read in `Update`: arrows and Ctrl shoot, Esc quits, numpad +/- zoom, B background, M music, R camera reset, A anti-aliasing, mouse rotates the scene. F5+F6 is the cheat. The debug console is the `²` key and expects a 1024x768 layout.

`game_lang`: `0` English, `1` French, `2` Spanish. User-facing strings go through `translate()` in `si_Functions.cpp`. Update all three languages together.

## State

Define new gameplay globals in `si_Main.cpp` and declare them `extern` in `si_Main.h`. Entity tables are fixed:

| Symbol | Capacity |
| --- | --- |
| `player` | 10 |
| `supership` | 10 |
| `ship` | 50 |
| `tir` | 51 (`max_tirs` caps simultaneous shots; the index wraps) |
| `brick` | 500 |
| `bonus` | 10 |
| `music` | 300 |
| `texture` | 100 |
| `explosion` | 60, each with `MAX_PARTICLES` (15) |

Raising a capacity means every index site, not only the declaration.

`DEBUG_MORTAL`, `DEBUG_MUSIC`, `DEBUG_BUNKERS`, and `DEBUG_FPS` are defined near the top of `si_Main.cpp`. Leave them as they are unless the task is about those debug paths.

## Assets

Art, audio, and models are compiled into the executable by `resource.rc`. A new asset needs the file on disk, a resource id in `resource.h`, and a matching entry in `resource.rc`. `si_Loading.cpp` is what actually binds textures and sounds at startup.

`si_Resources.cpp` writes some `BINARY` resources (`font.tga`, `.3ds` models) to `%TEMP%\SpaceInvaders-OpenGL` and can extract `fmod.dll`. Keep extract paths and resource ids in sync.

Hiscore and startup options persist under `HKLM\SOFTWARE\SpaceInvaders-OpenGL\` (`get_hiscore` / `set_hiscore` / `get_options` / `set_options`, and the startup dialog in `si_Basecode.cpp`). Writes to `HKLM` need elevation on current Windows.

Music streaming can open a network connection (Shoutcast `.pls`, a stream, or an mp3 URL). Startup can request `www.google.com` to bring the interface up. Treat that as existing behavior.

## Style

- Indent with tabs, width 4, as noted at the top of `si_Main.cpp`.
- Sources are Latin-1 (Windows-1252) with CRLF. Keep that encoding and those line endings in files you edit. `AGENTS.md` and other new Markdown can be UTF-8.
- Comments are French or English depending on the surrounding code. Follow the file you are in.
- Headers use the existing guards (`_MAIN_H`, `_BASECODE_H`, …) and backslash includes (`<gl\gl.h>`, `<Windows.h>`).
- `si_Font.c` stays C. Do not rename it.
- `si_Main.h` disables warnings 4305, 4244, and 4005. Do not churn types across the game to silence them.
- New `.cpp` / `.h` / `.c` files should carry the same GPL notice used at the top of `si_Main.cpp`.

## Leave alone

Do not reformat or modernize `si_Main.cpp`, `si_World.cpp`, `si_Basecode.cpp`, `si_fmod.h`, `gl/glext.h`, or `gl/wglext.h`. Do not replace fixed arrays, globals, or the Win32 loop with STL, RAII, or a different window/GL stack unless asked.

`si_fmod.h`, `si_fmod_errors.h`, `glpng.h`, `glpng.lib`, `fmodvc.lib`, `fmod.dll`, `gl/glext.h`, and `gl/wglext.h` are third-party. `resource.h` ids are shared with `resource.rc`; change both.

Do not add `Space Invaders.suo` or other per-user IDE files beyond what is already tracked.
