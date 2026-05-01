//
//  engine/config.h
//  Engine specific configurations
//  Copyright (c) 2025 Carlos Camacho. All rights reserved.
//

#ifndef CORGO_ENGINE_CONFIG_H
#define CORGO_ENGINE_CONFIG_H

// Game directory where game code is located
#ifndef CE_ENGINE_GAME_DIR
#define CE_ENGINE_GAME_DIR game
#endif // CE_ENGINE_GAME_DIR

// Include game config to override default settings
#include "utils/helpers.h"
#include CE_GET_GAME_INCLUDE(CE_ENGINE_GAME_DIR, config)


//// Engine configurations

// Asset cache initial size
#ifndef CE_ENGINE_ASSET_CACHE_INITIAL_SIZE
#define CE_ENGINE_ASSET_CACHE_INITIAL_SIZE 16
#endif // CE_ENGINE_ASSET_CACHE_INITIAL_SIZE


//// Display configurations

// Default refresh rate (0-50), may be changed in runtime by calling CE_Display_SetRefreshRate
#ifndef CE_ENGINE_REFRESH_RATE_DEFAULT
#define CE_ENGINE_REFRESH_RATE_DEFAULT 30
#endif // CE_ENGINE_REFRESH_RATE_DEFAULT

// Default display scale (1,2,4,8), may be changed in runtime by calling CE_Display_SetScale
#ifndef CE_ENGINE_SCALE_DEFAULT
#define CE_ENGINE_SCALE_DEFAULT 1
#endif // CE_ENGINE_SCALE_DEFAULT

// Enable adaptive rendering, which tries to minimize power use and CPU use by skipping rendering
#ifdef CE_ENGINE_ENABLE_ADAPTIVE_RENDERING
#define CE_ENGINE_ENABLE_ADAPTIVE_RENDERING 1
#endif // CE_ENGINE_ENABLE_ADAPTIVE_RENDERING


//// Input configurations

// Default input map size
#ifndef CE_ENGINE_INPUT_MAP_STACK_SIZE
#define CE_ENGINE_INPUT_MAP_STACK_SIZE 4
#endif // CE_ENGINE_INPUT_MAP_STACK_SIZE


//// Debug options (only for debug builds)

// Show FPS counter
#ifndef CE_SHOW_FPS
#define CE_SHOW_FPS 1
#endif // CE_SHOW_FPS

#endif // CORGO_ENGINE_CONFIG_H