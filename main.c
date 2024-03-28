/******************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for the XMC MCU: Empty Application
 *              Template for ModusToolbox.
 *
 * Related Document: See README.md
 *
 ******************************************************************************
 *
 * Copyright (c) 2015-2021, Infineon Technologies AG
 * All rights reserved.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#include "XMC1100.h"
#include "cy_utils.h"
#include "cybsp.h"
#include "cycfg_peripherals.h"
#include "modes_states_pins.h"
#include "xmc_ccu4.h"
#include <math.h>
#include <stdint.h>

// Operating mode variable
uint16_t mode = MODE_IDLE;
// State variable
uint16_t state = 0;

uint32_t lookupMatrix[4][4] = {{MODE_RL_OFF_OUT, MODE_RL_ON_OUT, MODE_RL_OFF_OUT, MODE_RL_ON_OUT},
                               {MODE_LR_OFF_OUT, MODE_LR_ON_OUT, MODE_LR_OFF_OUT, MODE_LR_ON_OUT},
                               {MODE_BP_OFF_OUT, MODE_BP_RL_ON_OUT, MODE_BP_OFF_OUT, MODE_BP_LR_ON_OUT},
                               {MODE_IDLE_OUT, MODE_IDLE_OUT, MODE_IDLE_OUT, MODE_IDLE_OUT}};

void setPeriod(double_t period) {
  // timer ticks = periodeValue in s * 10^9(convert into ns) / 2 (duty cycle 50 %) / 31.25 (time between timer ticks)
  uint32_t ticks = (uint32_t)round((period * pow(10, 9)) / 2.0 / 31.25);

  uint16_t msbValue = (uint16_t)(ticks >> 16);
  uint16_t lsbValue = (uint16_t)(ticks / (msbValue + 1));

  XMC_CCU4_SLICE_SetTimerPeriodMatch(timerLSB_HW, lsbValue);
  XMC_CCU4_SLICE_SetTimerPeriodMatch(timerMSB_HW, msbValue);
  XMC_CCU4_EnableShadowTransfer(ccu4_0_HW, (XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_SLICE_1));
}

void setFrequency(double_t frequency) {
  //Half the period to double the frequency because of switching directions 
  if (mode == MODE_BP)
    setPeriod(1.0 / frequency / 2);
  else
    setPeriod(1.0 / frequency);
}

void wait40ms() {
  XMC_CCU4_SLICE_StartTimer(timer40ms_HW);
  while (XMC_CCU4_SLICE_IsTimerRunning(timer40ms_HW))
    ;
  XMC_CCU4_SLICE_ClearTimer(timer40ms_HW);
}

void ccu4_0_SR0_INTERRUPT_HANDLER() {
  XMC_CCU4_SLICE_ClearEvent(timerMSB_HW, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

  PORT0->OMR = lookupMatrix[mode][state];
  // When last state is reached start from the beginning else increment state
  state = (state == 3) ? 0 : state + 1;
}

void HardFault_Handler() {
  PORT0->OMR = MODE_IDLE_OUT;
  while (true) {
  };
}

int main(void) {
  cy_rslt_t result;

  /* Initialize the device and board peripherals */
  result = cybsp_init();
  if (result != CY_RSLT_SUCCESS) {
    CY_ASSERT(0);
  }
  // set operating mode
  mode = MODE_BP;
  // Set default output
  PORT0->OMR = MODE_IDLE_OUT;

  NVIC_SetPriority(ccu4_0_SR0_IRQN, 0U);
  NVIC_EnableIRQ(ccu4_0_SR0_IRQN);

  setFrequency(200);

  for (;;) {
  }
}

/* [] END OF FILE */
