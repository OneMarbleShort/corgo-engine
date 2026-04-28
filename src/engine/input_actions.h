//
//  engine/input_actions.h
//  Main include for engine input actions.
//  Copyright (c) 2026 Carlos Camacho. All rights reserved.
//

#ifndef CORGO_ENGINE_INPUT_ACTIONS_H
#define CORGO_ENGINE_INPUT_ACTIONS_H

#include "engine/config.h"
#include "engine/components/input.h"

// Include game input actions
#include CE_GET_GAME_INCLUDE(CE_ENGINE_GAME_DIR, input_actions)
#ifndef CE_INPUT_ACTIONS_GAME
#define CE_INPUT_ACTIONS_GAME(CE_IA_DESC)
#endif // CE_INPUT_ACTIONS_GAME

typedef enum CE_INPUT_ACTIONS {
#define CE_IA_DESC(name) CE_IA(name),
    CE_INPUT_ACTIONS_GAME(CE_IA_DESC)
#undef CE_IA_DESC
    CE_ENGINE_INPUT_ACTIONS_COUNT // Invalid input action count
} CE_INPUT_ACTIONS;

/**
 * @brief Debug function to get the name of an input action as a string, used for debugging and logging purposes.
 * @param action[in] the input action enum value
 * @return a string literal with the name of the input action, or "Unknown" if the action is not recognized.
 */
const char* CE_Engine_GetInputActionNameDebugStr(IN CE_INPUT_ACTIONS action);

#endif // CORGO_ENGINE_INPUT_ACTIONS_H
