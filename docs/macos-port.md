# macOS port

Run the existing game on macOS. `Space Invaders.sln` stays the Windows build and is left unchanged.

The port is a new host around the current `si_*` sources. Gameplay tables, `Update`, `GameLogic`, and `Draw` stay in those files. Each checklist item ends with a binary that builds and does the thing that item describes. Finish one item before starting the next.

## Decisions

- CMake builds the macOS binary. Homebrew SDL2 (the `sdl2-compat` package) provides the window, input, and an OpenGL 2.1 compatibility context. macOS still has that legacy context, and this game’s immediate-mode drawing needs it. A 3.2+ core context would reject `glBegin`.
- Assets load from `images/`, `sounds/`, and `models/`. `resource.rc` is not compiled on macOS.
- Audio comes later, through SDL_mixer, because the game plays sound effects plus XM modules. `fmod.dll` and `fmodvc.lib` are 32-bit Windows libraries and are not linked.
- Hiscore and startup options go to a file under Application Support. The Windows registry path stays in the Win32 code.
- New macOS files are UTF-8. Existing `si_*` sources stay Latin-1 with CRLF. Do not reformat them while porting.
- Out of this pass until the game is playable: internet radio, the Wivik keyboard, the About dialog, and extracting `fmod.dll`.

## Build

```bash
brew install cmake sdl2
cmake -S . -B build
cmake --build build
./build/spaceinvaders
```

`SPACEINVADERS_SELFTEST=1 ./build/spaceinvaders` opens the window, prints the GL version, and exits. That is the check for item 1.

## Checklist

- [x] **1. Host window.** `platform/macos/host.cpp` opens a 640×480 window, creates an OpenGL 2.1 compatibility context, clears the frame, and exits on Escape or window close. The game sources are not linked yet. Verified with `SPACEINVADERS_SELFTEST=1`: `gl_version: 2.1 Metal - 90.5` on Apple M3.
- [x] **2. Compile the game.** `platform/macos/include/` supplies Win32, GL, and CRT declarations. `spaceinvaders_game` compiles all 13 `si_*.cpp` / `si_Font.c` objects. They are not linked into the executable yet. Four `#include` paths changed from `\` to `/` because Clang does not treat `\` as a separator, and `fontDrawString` takes `const char*` so ternary string literals type-check.
- [x] **3. Link.** `platform/macos/stubs.c` defines the Win32 and FMOD symbols the objects reference. `platform/macos/stubs.cpp` defines the OLE picture methods. `cmake --build build` produces `build/spaceinvaders`, and the item 1 self-test still prints the GL version.
- [x] **4. Enter the game.** The startup dialog is skipped. `platform/macos/game.cpp` fills a windowed 640×480 `GL_Window` and runs `Initialize` plus the `game_step` switch. A normal launch stays up. `SPACEINVADERS_SELFTEST=1` runs eight frames and prints `game_step`.
- [x] **5. Disk assets.** `FindResource` reads `images/`, `sounds/`, and `models/` using the paths in `resource.rc`. BMP, TGA, JPG, and PNG loads succeed. `DWORD` stays 32-bit so the texture pixel loops match the Windows layout.
- [ ] **6. Loading screen.** The load thread runs, the progress display shows, and `game_step` reaches 4.
- [ ] **7. Input.** Keyboard and mouse update `Keys::keyDown` and the existing camera drag. Arrows, Ctrl, and Esc do what `Readme.txt` says.
- [ ] **8. Playfield.** Player, invaders, bunkers, and shots draw in the SDL window.
- [ ] **9. Audio.** Sound effects and the XM module play through SDL_mixer. The M key still toggles music.
- [ ] **10. Save file.** Hiscore and the startup options persist in Application Support and survive a restart.
- [ ] **11. Play options.** VSync, anti-aliasing, background cycle, zoom, and the F5+F6 cheat work from the existing keys.

## Current step

Item 6. The load thread runs, the progress display swaps, and `game_step` reaches 4.
