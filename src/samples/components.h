//
//  samples/components.h
//  ECS components for the sample scenes
//  Copyright (c) 2025 Carlos Camacho. All rights reserved.
//

#ifndef CORGO_SAMPLES_COMPONENTS_H
#define CORGO_SAMPLES_COMPONENTS_H

#include "../ecs/core/component.h"

#include "scenes/scene_data.h"

#define CE_COMPONENT_DESC_GAME(CE_COMPONENT_DESC) \
    CE_COMPONENT_DESC(CE_TEXT_SCROLLER_SCENE_DATA_COMPONENT, 50, CE_TextScrollerSceneData, 1) \

    

#define CE_GLOBAL_COMPONENT_DESC_GAME(CE_GLOBAL_COMPONENT_DESC) \


#endif // CORGO_SAMPLES_COMPONENTS_H