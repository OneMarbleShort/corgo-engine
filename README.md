# Corgo Engine

A short-legged but complete ECS (Entity Component System) engine for the Playdate handheld console, written in pure C.
Intended to be a solid engine for writing most kinds of games, designed with flexibility in mind.

I am writing a devblog describing the design and development of the engine, you can read it here: https://medium.com/@mrkaru

**Status**: Ready for game development. Development Paused.

Corgo engine is currently at the "Engine builder" state: i.e. all the basic services are present but there is very little game specific functionality (graphics, sound, etc). A developer can build a customized engine based on Corgo, but they will have to build a lot of game specific components. 

Generic features like UI, tile maps, sounds, etc. will be added later. These will be generic and won't be as optimized as building them with a specific game in mind.

## Development has paused!

I am sad to announce that I will be pausing active development of Corgo Engine, I will be focusing on my profesional work for a while. Apologies.

Development will continue by One Marble Short (https://github.com/onemarbleshort/corgo-engine) that as of April 2026 is the sole steward of the engine.

## Requirements

- [Playdate SDK](https://play.date/dev/) (set `PLAYDATE_SDK_PATH` environment variable)
- [Visual Studio 2022 or 2026 (Version 17.9 or newer)](https://visualstudio.microsoft.com/) with C/C++ development tools. 
-- If using 2026: install VS2022 build tools (https://visualstudio.microsoft.com/vs/older-downloads/)
- [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) (arm-none-eabi-gcc 12.2) for device builds
- [CMake](https://cmake.org/) 3.14 or higher
- [VSCode](https://code.visualstudio.com/download) Recommended IDE. Install the following Plugins:
-- C/C++ support and DevTools: basic language support
-- CMake Tools: run builds and tests from the IDE
-- ELFInsights: debug the generated ELF images

## Demo
See captures of the engine here: https://github.com/karurosu/corgo-engine/tree/main/media

## Current state
1. ECS Core: 90%. Need to do an optimization pass once everything else is running
2. Asset loader: Done
4. SceneGraph: Done
5. Scene management: Done
6. Engine Main loop: Done
7. Graphics: Demo
8. Audio: nothing yet
9. Input: Done
11. Playdate backend: Demo
12. PC backend: nothing yet
13. UI: WIP
14. Entity Streamer: nothing yet

## Quickstart for Game Developers
Please use the template project to quickstart building a game: https://github.com/karurosu/corgo-engine-template

## Quick Start (for engine development)
### Setup (Required)
1. Install required tools
2. IMPORTANT: Add ARM Toolchain to PATH
3. Set `PLAYDATE_SDK_PATH` environment variable to your SDK installation path
4. Set up VScode and open the root dir as a project.

### CMake Presets (Recommended)
CMake is the preferred way to build the engine, Corgo Engine ships with multiple presets that build different variations:
- `game-vs2022-sim` -> Visual Studio Simulator with empty "game" project (`build.vs2022.game/`)
- `samples-vs2022-sim` -> Visual Studio Simulator with "samples" project (`build.vs2022.samples/`)
- `game-pd-debug` -> Playdate ARM debug configure with empty "game" project (`build.pd.game/`), uses `NMAKE_PATH` (see below)
- `game-pd-release` -> Playdate ARM release configure with empty "game" project (`build.pd.release.game/`), uses `NMAKE_PATH` (see below)
- `samples-pd-debug` -> Playdate ARM debug configure with "samples" project (`build.pd.samples/`), uses `NMAKE_PATH` (see below)
- `samples-pd-release` -> Playdate ARM release configure with "samples" project (`build.pd.release.samples/`), uses `NMAKE_PATH` (see below)

VSCode has a plugin called CMake Tools that can use the different presets from the IDE: https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-presets.md

### Device builds
In order to build using the pd-* presets `NMAKE_PATH` must be set and point to nmake.exe
A script is provided: `scripts/set-up-eenv-vars.ps1` that will set the variable by using vswhere to find the correct path.

This only needs to be run once (or if the build tools change)

A VSCode task has been provided to run from the IDE:
1. Click on the Quick Access Bar
2. Type 'task' (without quotes) and a space
3. Select 'Set Up Env Variables'

### Build from CLI
Cmake presets can run from a CLI: https://gist.github.com/MangaD/1368e9a5a7232a99fe61cb6154df8e2d

## Multiple Games
Corgo Engine supports building multiple games from the same source directory. Each game has its own C files, own components, systems, etc. But all share the same assets. This is useful for prototyping multiple games at the same time.

See the template for more information.

## Debugging
Corgo Engine ships with a launch.json file that can launch the game from VSCode and attach a debugger.
Similar functionality can be used from Visual Studio but it has not been fully configured to use presets.

## Running tests
The engine has a small suite of unit tests for the core ECS and tools based on Unity Test. 
To run:
1. Build the coretest project
2. Run coretest.exe or use the CMake preset

## 3rd Party Credits

Convenient Containers by JacksonAllan.
MIT License
https://github.com/JacksonAllan/CC?tab=readme-ov-file

Unity Test by ThrowInTheSwitch
MIT License
https://github.com/ThrowTheSwitch/Unity?tab=readme-ov-file

printf by Eyal Rozenberg
MIT License
https://github.com/eyalroz/printf

## License

BSD 3-Clause License. See `LICENSE` for full text.
