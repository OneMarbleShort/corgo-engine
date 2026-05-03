//
//  engine/sample_scenes/sample_menu.c
//  A menu to select wwhich sample to load.
//  Copyright (c) 2026 Carlos Camacho. All rights reserved.
//

#include "engine/corgo.h"
#include "engine/shortcuts/scene.h"

// Move to a component
static int demo = -1;

CE_DECLARE_SCENE_CREATE_FUNCTION(SampleMenu)
{
    return CE_OK;
}

CE_DECLARE_SCENE_RUN_FUNCTION(SampleMenu)
{
    if (demo == -1)
    {
        // Load first scene immediately
        CES_LOAD_SCENE(TextScroller);
        demo = 0;
        return CE_OK;
    }

    demo = (demo + 1) % 2;
    if (demo == 0) {
        CES_LOAD_SCENE(TextScroller);
    } else {
        CES_LOAD_SCENE(TextScroller2);
    }

    return CE_OK;
}

CE_GENERATE_SCENE(SampleMenu, CE_INVALID_TYPE_ID)
