#include "modes_states_pins.h"

const output_t lookupMatrix[4][4] = {{MODE_RL_OFF_OUT, MODE_RL_ON_OUT, MODE_RL_OFF_OUT, MODE_RL_ON_OUT},
                                     {MODE_LR_OFF_OUT, MODE_LR_ON_OUT, MODE_LR_OFF_OUT, MODE_LR_ON_OUT},
                                     {MODE_BP_OFF_OUT, MODE_BP_RL_ON_OUT, MODE_BP_OFF_OUT, MODE_BP_LR_ON_OUT},
                                     {MODE_IDLE_OUT, MODE_IDLE_OUT, MODE_IDLE_OUT, MODE_IDLE_OUT}};