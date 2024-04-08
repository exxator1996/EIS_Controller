#ifndef MODES_STATES_PINS_H
#define MODES_STATES_PINS_H

#include <XMC1100.h>
//TODO Change Macros to enums
// Operating modes

#define MODE_RL   0
#define MODE_LR   1
#define MODE_BP   2
#define MODE_IDLE 3

// Output pin setter Traco 1
#define PIN_PRI_1_ON   PORT0_OMR_PS7_Msk
#define PIN_SEK_1_ON   PORT0_OMR_PS6_Msk
#define PIN_CRTL_1_ON  PORT0_OMR_PS8_Msk
// Output pin setter Traco 2
#define PIN_PRI_2_ON   PORT0_OMR_PS14_Msk
#define PIN_SEK_2_ON   PORT0_OMR_PS15_Msk
#define PIN_CRTL_2_ON  PORT0_OMR_PS9_Msk
// Output pin resetter Traco 1
#define PIN_PRI_1_OFF  PORT0_OMR_PR7_Msk
#define PIN_SEK_1_OFF  PORT0_OMR_PR6_Msk
#define PIN_CRTL_1_OFF PORT0_OMR_PR8_Msk
// Output pin resetter Traco 2
#define PIN_PRI_2_OFF  PORT0_OMR_PR14_Msk
#define PIN_SEK_2_OFF  PORT0_OMR_PR15_Msk
#define PIN_CRTL_2_OFF PORT0_OMR_PR9_Msk

// Output states MODE_IDLE (all off)
#define MODE_IDLE_OUT     (PIN_PRI_1_OFF | PIN_SEK_1_OFF | PIN_CRTL_1_OFF | PIN_PRI_2_OFF | PIN_SEK_2_OFF | PIN_CRTL_2_OFF)
// Output states MODE_RL
#define MODE_RL_OFF_OUT   (PIN_PRI_1_ON | PIN_SEK_1_OFF | PIN_CRTL_1_ON | PIN_PRI_2_OFF | PIN_SEK_2_OFF | PIN_CRTL_2_OFF)
#define MODE_RL_ON_OUT    (PIN_PRI_1_ON | PIN_SEK_1_ON | PIN_CRTL_1_ON | PIN_PRI_2_OFF | PIN_SEK_2_OFF | PIN_CRTL_2_OFF)
// Output state MODE_LR _PS
#define MODE_LR_OFF_OUT   (PIN_PRI_1_OFF | PIN_SEK_1_OFF | PIN_CRTL_1_OFF | PIN_PRI_2_ON | PIN_SEK_2_OFF | PIN_CRTL_2_ON)
#define MODE_LR_ON_OUT    (PIN_PRI_1_OFF | PIN_SEK_1_OFF | PIN_CRTL_1_OFF | PIN_PRI_2_ON | PIN_SEK_2_ON | PIN_CRTL_2_ON)
// Output state MODE_BP _PS
#define MODE_BP_OFF_OUT   (PIN_PRI_1_ON | PIN_SEK_1_OFF | PIN_CRTL_1_ON | PIN_PRI_2_ON | PIN_SEK_2_OFF | PIN_CRTL_2_ON)
#define MODE_BP_RL_ON_OUT (PIN_PRI_1_ON | PIN_SEK_1_ON | PIN_CRTL_1_ON | PIN_PRI_2_ON | PIN_SEK_2_OFF | PIN_CRTL_2_ON)
#define MODE_BP_LR_ON_OUT (PIN_PRI_1_ON | PIN_SEK_1_OFF | PIN_CRTL_1_ON | PIN_PRI_2_ON | PIN_SEK_2_ON | PIN_CRTL_2_ON)


#endif
