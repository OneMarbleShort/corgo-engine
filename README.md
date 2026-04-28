# Corgo Engine

A short-legged but complete ECS (Entity Component System) engine for the Playdate handheld console, written in pure C.
Intended to be a solid engine for writing most kinds of games, designed with flexibility in mind.

I am writing a devblog describing the design and development of the engine, you can read it here: https://medium.com/@mrkaru

**Status**: Ready for game development.

Corgo engine is currently at the "Engine builder" state: i.e. all the basic services are present but there is very little game specific functionality (graphics, sound, etc)
A developer can build a customized engine based on Corgo. The reasoning is that a small console like the Playdate requires every ounce of performance, so it is much better for a developer to tailor specific functionality to the game. 
Generic features like UI, tile maps, sounds, etc. are being worked on and added later, but they may have performance tradeoffs in order to work in a wide range of cases.

## Requirements

- [Playdate SDK](https://play.date/dev/) (set `PLAYDATE_SDK_PATH` environment variable)
- [Visual Studio 2022 or 2026](https://visualstudio.microsoft.com/) with C/C++ development tools. 
-- If using 2026: install VS2022 build tools (https://visualstudio.microsoft.com/vs/older-downloads/)
- [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) (arm-none-eabi-gcc 12.2) for device builds
- [CMake](https://cmake.org/) 3.14 or higher

## Demo
See the early version running in the simulator here: 
https://github.com/karurosu/corgo-engine/blob/main/demo.mp4?raw=true

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

## Projects
Corgo Engine supports building multiple game from the same source directory. These are called "Projects".
A project is a collection of .h/.c files for a game that are only built if the project is enabled. 
Note that currently all assets under Source/ are added to all PDX files, regardless of project, so this feature is only to simplify development.
This will be fixed in a later release.

See the CMakePresets for more details on how to add a new project.

## Quick Start (for engine development)
### Setup (Required))
1. Install required tools
2. IMPORTANT: Add ARM Toolchain to PATH
3. Set `PLAYDATE_SDK_PATH` environment variable to your SDK installation path

### CMake Presets (Recommended)
Available Cmake presets for use within VSCode (or via CLI):
- `vs2022-sim` -> Visual Studio Simulator with default project (`build.vs2022/`)
- `game-vs2022-sim` -> Visual Studio Simulator with default "game" project (`build.vs2022.game/`)
- `samples-vs2022-sim` -> Visual Studio Simulator with "samples" project (`build.vs2022.samples/`)
- `pd-debug` -> Playdate ARM debug configure (`build.pd/`), uses `NMAKE_PATH` (see below)
- `pd-release` -> Playdate ARM release configure (`build.pd.release/`), uses `NMAKE_PATH` (see below)
- `game-pd-debug` -> Playdate ARM debug configure with "game" project (`build.pd.game/`), uses `NMAKE_PATH` (see below)
- `game-pd-release` -> Playdate ARM release configure with "game" project (`build.pd.release.game/`), uses `NMAKE_PATH` (see below)
- `samples-pd-debug` -> Playdate ARM debug configure with "samples" project (`build.pd.samples/`), uses `NMAKE_PATH` (see below)
- `samples-pd-release` -> Playdate ARM release configure with "samples" project (`build.pd.release.samples/`), uses `NMAKE_PATH` (see below)

### Device builds
In order to build using the pd-* presets `NMAKE_PATH` must be set and point to VS nmake.exe.
A script is provided: `scripts/device-cmake.ps1` that will set the right configuration.

A CMake 'Configure Task' is included that can be run from CLI or VSCode.

### Building Manually (Not recommended)
Some simple batch scrips are provided. These will only build the 'game' projects and it is recommended to use the Cmake presets instead.

- `UpdateSolutions.bat` - Update cmake configurations
- `BuildNMake.bat` - Build simulator version
- `BuildDevice.bat` - Build device debug version
- `BuildDeviceRetail.bat` - Build device release version

### Debugging
- Build and run in VS Code:
-- There are 2 run configuration, one for "game" and one for "samples"
- Open `build.vs2022/corgo_engine.sln` in Visual Studio 2022 and press F5 to build and launch the Playdate Simulator.

### Running tests
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
