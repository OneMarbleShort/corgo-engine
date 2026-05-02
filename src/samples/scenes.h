//
//  samples/scenes.h
//  Main include for samples demo scenes.
//  Copyright (c) 2026 Carlos Camacho. All rights reserved.
//

#ifndef CORGO_SAMPLES_SCENES_H
#define CORGO_SAMPLES_SCENES_H

#include "engine/core/scene.h"

CE_DECLARE_SCENE(SampleMenu) // Main scene selector menu
CE_DECLARE_SCENE(TextScroller) // A simple scene that scrolls text
CE_DECLARE_SCENE(TextScroller2) // An alternative text scroller
CE_DECLARE_SCENE(InputDemo) // A scene to demonstrate input handling and action maps
CE_DECLARE_SCENE(CrankDemo) // A scene to demonstrate crank dock state and angle
CE_DECLARE_SCENE(SpriteDemo)  // A scene to demonstrate sprite rendering and animation

// Set this to the scene you want to load first
#ifndef CE_ENGINE_SET_START_SCENE
#define CE_ENGINE_SET_START_SCENE SpriteDemo
#endif

#endif // CORGO_SAMPLES_SCENES_H
