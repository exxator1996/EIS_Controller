#ifndef MODES_STATES_PINS_H
#define MODES_STATES_PINS_H

#include <XMC1100.h>
// Operating modes
#define MODE_RL        0x0
#define MODE_LR        0x1
#define MODE_BP        0x3
#define MODE_IDLE      0x4
// State Idle
#define MODE_IDLE_OFF  0x5
// States MODE RL
#define MODE_RL_OFF    0x6
#define MODE_RL_ON     0x7
// States MODE LR
#define MODE_LR_OFF    0x8
#define MODE_LR_ON     0x9
// States MODE BP
#define MODE_BP_RL_OFF 0x10
#define MODE_BP_RL_ON  0x11
#define MODE_BP_LR_OFF 0x12
#define MODE_BP_LR_ON  0x13

// Output pin setter Traco 1
#define PIN_PRI_1_PS  PORT0_OMR_PS0_Msk
#define PIN_SEK_1_PS  PORT0_OMR_PS5_Msk
#define PIN_CRTL_1_PS PORT0_OMR_PS6_Msk
// Output pin setter Traco 2
#define PIN_PRI_2_PS  PORT0_OMR_PS7_Msk
#define PIN_SEK_2_PS  PORT0_OMR_PS8_Msk
#define PIN_CRTL_2_PS PORT0_OMR_PS9_Msk
// Output pin resetter Traco 1
#define PIN_PRI_1_PR  PORT0_OMR_PR0_Msk
#define PIN_SEK_1_PR  PORT0_OMR_PR5_Msk
#define PIN_CRTL_1_PR PORT0_OMR_PR6_Msk
// Output pin resetter Traco 2
#define PIN_PRI_2_PR  PORT0_OMR_PR7_Msk
#define PIN_SEK_2_PR  PORT0_OMR_PR8_Msk
#define PIN_CRTL_2_PR PORT0_OMR_PR9_Msk

// Output states MODE_IDLE (all off)
#define MODE_IDLE_OFF_OUT (PIN_PRI_1_PR | PIN_SEK_1_PR | PIN_CRTL_1_PR | PIN_PRI_2_PR | PIN_SEK_2_PR | PIN_CRTL_2_PR)
// Output states MODE_RL _PS = HIGH _PR = LOW
#define MODE_RL_OFF_OUT   (PIN_PRI_1_PS | PIN_SEK_1_PR | PIN_CRTL_1_PS | PIN_PRI_2_PR | PIN_SEK_2_PR | PIN_CRTL_2_PR)
#define MODE_RL_ON_OUT    (PIN_PRI_1_PS | PIN_SEK_1_PS | PIN_CRTL_1_PS | PIN_PRI_2_PR | PIN_SEK_2_PR | PIN_CRTL_2_PR)
// Output state MODE_LR _PS = HIGH _PR = LOW
#define MODE_LR_OFF_OUT   (PIN_PRI_1_PR | PIN_SEK_1_PR | PIN_CRTL_1_PR | PIN_PRI_2_PS | PIN_SEK_2_PR | PIN_CRTL_2_PS)
#define MODE_LR_ON_OUT    (PIN_PRI_1_PR | PIN_SEK_1_PR | PIN_CRTL_1_PR | PIN_PRI_2_PS | PIN_SEK_2_PS | PIN_CRTL_2_PS)
// Output state MODE_BP _PS = HIGH _PR = LOW
#define MODE_BP_OFF_OUT   (PIN_PRI_1_PS | PIN_SEK_1_PR | PIN_CRTL_1_PS | PIN_PRI_2_PS | PIN_SEK_2_PR | PIN_CRTL_2_PS)
#define MODE_BP_RL_ON_OUT (PIN_PRI_1_PS | PIN_SEK_1_PS | PIN_CRTL_1_PS | PIN_PRI_2_PS | PIN_SEK_2_PR | PIN_CRTL_2_PS)
#define MODE_BP_LR_ON_OUT (PIN_PRI_1_PS | PIN_SEK_1_PR | PIN_CRTL_1_PS | PIN_PRI_2_PS | PIN_SEK_2_PS | PIN_CRTL_2_PS)

#endif
