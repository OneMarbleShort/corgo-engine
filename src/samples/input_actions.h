//
//  samples/input_actions.h
//  Main include for sample input actions.
//  Copyright (c) 2026 Carlos Camacho. All rights reserved.
//

#ifndef CORGO_SAMPLES_INPUT_ACTIONS_H
#define CORGO_SAMPLES_INPUT_ACTIONS_H

// Sample input actions
#define CE_INPUT_ACTIONS_GAME(CE_IA_DESC) \
    CE_IA_DESC(DEMO_PRESS)\
    CE_IA_DESC(DEMO_RELEASE)\
    CE_IA_DESC(DEMO_HELD)\
    CE_IA_DESC(NEXT_INPUT_MAP)\
    CE_IA_DESC(PREVIOUS_INPUT_MAP)\
    CE_IA_DESC(DEMO_HELD_2)


// Sample Action Maps
CE_DECLARE_ACTION_MAP_FUNCTION(SampleInputMap1);
CE_DECLARE_ACTION_MAP_FUNCTION(SampleInputMap2);
    
#endif // CORGO_SAMPLES_INPUT_ACTIONS_H
