# macOS port

Run the existing game on macOS. `Space Invaders.sln` stays the Windows build and is left unchanged.

The port is a new host around the current `si_*` sources. Gameplay tables, `Update`, `GameLogic`, and `Draw` stay in those files. Each checklist item ends with a binary that builds and does the thing that item describes. Finish one item before starting the next.

## Decisions

- CMake builds the macOS binary. Homebrew SDL2 (the `sdl2-compat` package) provides the window, input, and an OpenGL 2.1 compatibility context. macOS still has that legacy context, and this game’s immediate-mode drawing needs it. A 3.2+ core context would reject `glBegin`.
- Assets load from `images/`, `sounds/`, and `models/`. `resource.rc` is not compiled on macOS.
- Audio is SDL_mixer. It plays the MP3 effects and the XM module. `fmod.dll` and `fmodvc.lib` are 32-bit Windows libraries and are not linked.
- Hiscore and startup options go to a file under Application Support. The Windows registry path stays in the Win32 code.
- New macOS files are UTF-8. Existing `si_*` sources stay Latin-1 with CRLF. Do not reformat them while porting.
- Not in this port: internet radio, the Wivik keyboard, the About dialog, and extracting `fmod.dll`.

## Build

```bash
brew install cmake sdl2 sdl2_mixer
cmake -S . -B build
cmake --build build
./build/spaceinvaders
```

`SPACEINVADERS_SELFTEST=1 ./build/spaceinvaders` loads into gameplay, prints the checks below, and exits after three gameplay frames or 20 seconds. F12 toggles between that 640×480 window and the desktop.

## Checklist

- [x] **1. Host window.** `platform/macos/host.cpp` opens a 640×480 window, creates an OpenGL 2.1 compatibility context, and exits on Escape or window close. The game sources are linked into `build/spaceinvaders`. Verified with `SPACEINVADERS_SELFTEST=1`: `gl_version: 2.1 Metal - 90.5` on Apple M3.
- [x] **2. Compile the game.** `platform/macos/include/` supplies Win32, GL, and CRT declarations. `spaceinvaders_game` compiles all 13 `si_*.cpp` / `si_Font.c` objects, and those objects are linked into the executable. Four `#include` paths changed from `\` to `/` because Clang does not treat `\` as a separator, and `fontDrawString` takes `const char*` so ternary string literals type-check.
- [x] **3. Link.** `platform/macos/stubs.c` defines the Win32 and FMOD symbols the objects reference. `platform/macos/stubs.cpp` defines the OLE picture methods. `cmake --build build` produces `build/spaceinvaders`, and the self-test prints the GL version on the way into gameplay.
- [x] **4. Enter the game.** The startup dialog is skipped. `platform/macos/game.cpp` fills a windowed 640×480 `GL_Window` and runs `Initialize` plus the `game_step` switch. A normal launch stays up. `SPACEINVADERS_SELFTEST=1` runs until three gameplay frames, or 20 seconds, and prints `game_step`.
- [x] **5. Disk assets.** `FindResource` reads `images/`, `sounds/`, and `models/` using the paths in `resource.rc`. BMP, TGA, JPG, and PNG loads succeed. `DWORD` stays 32-bit so the texture pixel loops match the Windows layout.
- [x] **6. Loading screen.** `_beginthread` starts the loader on a pthread. The loader swaps its own buffers while `game_step` is 2. `SPACEINVADERS_SELFTEST=1` prints `game_step: 4`.
- [x] **7. Input.** SDL key events fill `Keys::keyDown` with the game's virtual-key codes. Left mouse drag updates `angle_x` and `angle_y`. Escape ends the loop. The self-test reports `input_left: 1`.
- [x] **8. Playfield.** After three gameplay frames the self-test reads a 20-pixel grid and reports `playfield_samples: 560`, so the scene is on screen.
- [x] **9. Audio.** SDL_mixer plays the XM module and the MP3 effects. The self-test prints `music_playing: 1` and `sample_loaded: 1`. The M key still calls `FMUSIC_StopSong` / `FMUSIC_PlaySong`.
- [x] **10. Save file.** Registry reads and writes go to `~/Library/Application Support/SpaceInvaders-OpenGL/save.bin`. The self-test prints `save_roundtrip: 77` and leaves that file on disk.
- [x] **11. Play options.** The window asks for 4 multisample buffers (`aa_samples: 4`). `wglSwapIntervalEXT` calls `SDL_GL_SetSwapInterval` (`vsync: 1`). B, M, R, A, V, numpad zoom, and F5+F6 go through the same `Update` paths as on Windows. F12 toggles macOS fullscreen and restores the 640×480 window.

## Current step

The checklist is complete. `./build/spaceinvaders` is the macOS game. `SPACEINVADERS_SELFTEST=1 ./build/spaceinvaders` loads to gameplay and prints the checks above.
