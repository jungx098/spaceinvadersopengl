# macOS port

Run the existing game on macOS. `Space Invaders.sln` stays the Windows build and is left unchanged.

The port is a host around the current `si_*` sources. Gameplay tables, `Update`, `GameLogic`, and `Draw` stay in those files.

## Build

```bash
brew install cmake sdl2 sdl2_mixer
cmake -S . -B build
cmake --build build
./build/spaceinvaders
```

`SPACEINVADERS_SELFTEST=1 ./build/spaceinvaders` loads into gameplay and exits after three gameplay frames, or 20 seconds.

## What the host does

`platform/macos/host.cpp` opens a 640×480 window with an OpenGL 2.1 compatibility context. SDL feeds the game's existing keyboard and mouse controls. SDL_mixer plays the XM module and the MP3 effects. Hiscore and startup options go to `~/Library/Application Support/SpaceInvaders-OpenGL/save.bin`. F12 toggles fullscreen and restores the 640×480 window. Escape quits. The Windows startup dialog is skipped.

Assets load from `images/`, `sounds/`, and `models/` using the paths in `resource.rc`. That resource script is not compiled on macOS.

## What stays Windows

`Space Invaders.sln` is the Windows build. `fmod.dll` and `fmodvc.lib` are 32-bit Windows libraries and are not linked. The Windows registry path stays in the Win32 code.

## What is not ported

Internet radio, the About dialog, and the Wivik keyboard.
