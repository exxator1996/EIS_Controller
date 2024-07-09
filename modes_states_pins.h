#ifndef MODES_STATES_PINS_H
#define MODES_STATES_PINS_H

#include <XMC1100.h>

//  Operating modes
typedef enum { MODE_RL, MODE_LR, MODE_BP, MODE_IDLE } mode_t;

typedef enum {
  PIN_PRI_1_ON  = PORT0_OMR_PS7_Msk,
  PIN_SEK_1_ON  = PORT0_OMR_PS15_Msk,
  PIN_CRTL_1_ON = PORT0_OMR_PS8_Msk,
  PIN_PRI_2_ON  = PORT0_OMR_PS14_Msk,
  PIN_SEK_2_ON  = PORT0_OMR_PS6_Msk,
  PIN_CRTL_2_ON = PORT0_OMR_PS9_Msk
} pin_setter_t;

typedef enum {
  PIN_PRI_1_OFF  = PORT0_OMR_PR7_Msk,
  PIN_SEK_1_OFF  = PORT0_OMR_PR15_Msk,
  PIN_CRTL_1_OFF = PORT0_OMR_PR8_Msk,
  PIN_PRI_2_OFF  = PORT0_OMR_PR14_Msk,
  PIN_SEK_2_OFF  = PORT0_OMR_PR6_Msk,
  PIN_CRTL_2_OFF = PORT0_OMR_PR9_Msk
} pin_resetter_t;

typedef enum {
  // MODE_IDLE (all off)
  MODE_IDLE_OUT     = (PIN_PRI_1_OFF | PIN_SEK_1_OFF | PIN_CRTL_1_OFF | PIN_PRI_2_OFF | PIN_SEK_2_OFF | PIN_CRTL_2_OFF),
  // MODE_RL_OFF_OUT   (Traco 1 off & Traco 2 idle)
  MODE_RL_OFF_OUT   = (PIN_PRI_1_OFF | PIN_SEK_1_OFF | PIN_CRTL_1_OFF | PIN_PRI_2_ON | PIN_SEK_2_OFF | PIN_CRTL_2_ON),
  // MODE_RL_ON_OUT    (Traco 1 off & Traco 2 output)
  MODE_RL_ON_OUT    = (PIN_PRI_1_OFF | PIN_SEK_1_OFF | PIN_CRTL_1_OFF | PIN_PRI_2_ON | PIN_SEK_2_ON | PIN_CRTL_2_ON),
  // MODE_LR_OFF_OUT   (Traco 1 idle & Traco 2 off )
  MODE_LR_OFF_OUT   = (PIN_PRI_1_ON | PIN_SEK_1_OFF | PIN_CRTL_1_ON | PIN_PRI_2_OFF | PIN_SEK_2_OFF | PIN_CRTL_2_OFF),
  // MODE_LR_ON_OUT    (Traco 1 idle & Traco 2 off)
  MODE_LR_ON_OUT    = (PIN_PRI_1_ON | PIN_SEK_1_ON | PIN_CRTL_1_ON | PIN_PRI_2_OFF | PIN_SEK_2_OFF | PIN_CRTL_2_OFF),
  // MODE_BP_OFF_OUT   (Traco 1 idle & Traco 2 idle )
  MODE_BP_OFF_OUT   = (PIN_PRI_1_ON | PIN_SEK_1_OFF | PIN_CRTL_1_ON | PIN_PRI_2_ON | PIN_SEK_2_OFF | PIN_CRTL_2_ON),
  // MODE_BP_RL_ON_OUT (Traco 1 idle & Traco 2 output)
  MODE_BP_RL_ON_OUT = (PIN_PRI_1_ON | PIN_SEK_1_OFF | PIN_CRTL_1_ON | PIN_PRI_2_ON | PIN_SEK_2_ON | PIN_CRTL_2_ON),
  // MODE_BP_LR_ON_OUT (Traco 1 output & Traco 2 idle)
  MODE_BP_LR_ON_OUT = (PIN_PRI_1_ON | PIN_SEK_1_ON | PIN_CRTL_1_ON | PIN_PRI_2_ON | PIN_SEK_2_OFF | PIN_CRTL_2_ON)
} output_t;

#endif
