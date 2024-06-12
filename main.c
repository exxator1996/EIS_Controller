/******************************************************************************
 * File Name:   main.c
 *
 * Description: main.c for the EIS Controller board
 *
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2015-2021, Infineon Technologies AG All rights reserved.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 *accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 *and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 *Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 *and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all derivative
 *works of the Software, unless such copies or derivative works are solely in the form of machine-executable object code
 *generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 *COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 *CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 *
 *****************************************************************************/

#include "XMC1100.h"
#include "cy_utils.h"
#include "cybsp.h"
#include "cycfg_peripherals.h"
#include "modes_states_pins.h"
#include "xmc1_scu.h"
#include "xmc_ccu4.h"
#include "xmc_scu.h"
#include "xmc_uart.h"
#include "xmc_usic.h"
#include "xmc_wdt.h"
#include <math.h>
#include <stdint.h>

// Operating mode variable
mode_t mode    = MODE_IDLE;
// State variable
uint16_t state = 0;

output_t lookupMatrix[4][4] = {{MODE_RL_OFF_OUT, MODE_RL_ON_OUT, MODE_RL_OFF_OUT, MODE_RL_ON_OUT},
                               {MODE_LR_OFF_OUT, MODE_LR_ON_OUT, MODE_LR_OFF_OUT, MODE_LR_ON_OUT},
                               {MODE_BP_OFF_OUT, MODE_BP_RL_ON_OUT, MODE_BP_OFF_OUT, MODE_BP_LR_ON_OUT},
                               {MODE_IDLE_OUT, MODE_IDLE_OUT, MODE_IDLE_OUT, MODE_IDLE_OUT}};

void startTimers(void) {
  XMC_CCU4_SLICE_StartTimer(timerMSB_HW);
  XMC_CCU4_SLICE_StartTimer(timerLSB_HW);
  XMC_CCU4_SLICE_StartTimer(timerPeriodCount_HW);
}

void stopTimers(void) {
  XMC_CCU4_SLICE_StopClearTimer(timerMSB_HW);
  XMC_CCU4_SLICE_StopClearTimer(timerLSB_HW);
  XMC_CCU4_SLICE_StopClearTimer(timerPeriodCount_HW);
}

void setPeriodTime(double_t const period, double_t const dutyCycle) {
  // timer ticks period = periodeValue in s * 10^9(convert into ns) / 15.625 (time between timer ticks)
  uint32_t ticksPeriod  = (uint32_t)round((period * pow(10, 9)) / 15.625) - 1;
  // timer ticks compare output on for (duty cycle in %)
  uint32_t ticksCompare = (uint32_t)round(ticksPeriod * dutyCycle / 100.0) - 1;

  uint16_t periodMsbValue = (uint16_t)(ticksPeriod >> 16);
  uint16_t periodLsbValue = (uint16_t)(ticksPeriod / (periodMsbValue + 1));

  uint16_t compareMsbValue = (uint16_t)(ticksCompare >> 16);
  uint16_t compareLsbValue = (uint16_t)(ticksCompare / (compareMsbValue + 1));

  XMC_CCU4_SLICE_SetTimerCompareMatch(timerLSB_HW, compareLsbValue);
  XMC_CCU4_SLICE_SetTimerCompareMatch(timerMSB_HW, compareMsbValue);

  XMC_CCU4_SLICE_SetTimerPeriodMatch(timerLSB_HW, periodLsbValue);
  XMC_CCU4_SLICE_SetTimerPeriodMatch(timerMSB_HW, periodMsbValue);

  XMC_CCU4_EnableShadowTransfer(ccu4_0_HW, (XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_SLICE_1));
}

void setFrequency(double_t const frequency, double_t const dutyCycle) {
  // Only values between 100 mHz and 10 kHz
  if (frequency >= 0.1 && frequency <= 10000) {
    // Half the period to double the frequency because of switching directions
    if (mode == MODE_BP)
      setPeriodTime(1.0 / frequency / 2.0, 2.0);
    else
      setPeriodTime(1.0 / frequency, dutyCycle);
  }
}

void setPeriodCount(uint32_t const periodCountValue) {
  if (mode == MODE_RL || mode == MODE_LR) {
    XMC_CCU4_SLICE_SetTimerPeriodMatch(timerPeriodCount_HW, (2 * periodCountValue + 1));
  } else if (mode == MODE_BP) {
    XMC_CCU4_SLICE_SetTimerPeriodMatch(timerPeriodCount_HW, (4 * periodCountValue + 1));
  }
  XMC_CCU4_EnableShadowTransfer(ccu4_0_HW, XMC_CCU4_SHADOW_TRANSFER_SLICE_2);
}

void stopStimulation(void) {
  stopTimers();
  PORT0->OMR = lookupMatrix[mode][0];
}

void ccu4_0_SR0_INTERRUPT_HANDLER() {
  // Change output dependt on state and mode
  XMC_CCU4_SLICE_ClearEvent(timerMSB_HW, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);

  PORT0->OMR = lookupMatrix[mode][state];
  // When last state is reached start from the beginning else increment state
  state      = (state == 3) ? 0 : state + 1;
}

void ccu4_0_SR1_INTERRUPT_HANDLER() {
  // Change output dependt on state and mode
  XMC_CCU4_SLICE_ClearEvent(timerMSB_HW, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

  PORT0->OMR = lookupMatrix[mode][state];
  // When last state is reached start from the beginning else increment state
  state      = (state == 3) ? 0 : state + 1;
}

void ccu4_0_SR2_INTERRUPT_HANDLER() {
  // Interupt handler after periodCountValue is reached (Increased for each call of SR0 and SR1 INTERRUPT_HANDLER)
  XMC_CCU4_SLICE_ClearEvent(timerPeriodCount_HW, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);

  PORT0->OMR = MODE_IDLE_OUT;
  mode       = MODE_IDLE;

  stopTimers();
}

void uart_RECEIVE_BUFFER_STANDARD_EVENT_HANDLER() {
  XMC_USIC_CH_RXFIFO_ClearEvent(uart_HW, XMC_USIC_CH_RXFIFO_EVENT_STANDARD);

  uint8_t receivedData[4];
  uint8_t rxIndex = 0;
  // Read until recive Buffer is empty
  while (!XMC_USIC_CH_RXFIFO_IsEmpty(uart_HW)) {
    receivedData[rxIndex++] = XMC_UART_CH_GetReceivedData(uart_HW);
  }
  // Put together the new Frequency from received Data
  uint16_t newFrequency = (receivedData[2] << 8) + receivedData[3];

  //Transform recieved value into duty cycle
  double_t dutyCycle = receivedData[1] / 1.0;

  // Symbolic value FFFF to set 0.1 Hz and 0 to stop
  if (newFrequency == 0xFFFF) {
    setFrequency(0.1,dutyCycle);
    startTimers();
  } else if (newFrequency == 0xFFFE) {
    setFrequency(0.4,dutyCycle);
    startTimers();
  } else if (newFrequency == 0xFFFD) {
    setFrequency(0.8,dutyCycle);
    startTimers();
  } else if (newFrequency == 0x0000) {
    stopStimulation();
  } else {
    setFrequency((double_t)newFrequency,dutyCycle);
    startTimers();
  }
}

void HardFault_Handler() {
  PORT0->OMR = MODE_IDLE_OUT;
  while (true) {
  }
}

int main(void) {
  cy_rslt_t result;

  /* Initialize the device and board
   * peripherals */
  result = cybsp_init();
  if (result != CY_RSLT_SUCCESS) {
    CY_ASSERT(0);
  }

  // If last reset caused by watchdog
  // timer
  if (XMC_SCU_RESET_REASON_WATCHDOG & XMC_SCU_RESET_GetDeviceResetReason()) {
    PORT0->OMR = MODE_IDLE_OUT;
    XMC_SCU_RESET_ClearDeviceResetReason();
    // Prototype
  }

  // set operating mode
  mode       = MODE_RL;
  // Set default output
  PORT0->OMR = MODE_IDLE_OUT;

  NVIC_SetPriority(ccu4_0_SR0_IRQN, 0U);
  NVIC_EnableIRQ(ccu4_0_SR0_IRQN);

  NVIC_SetPriority(ccu4_0_SR1_IRQN, 0U);
  NVIC_EnableIRQ(ccu4_0_SR1_IRQN);

  // NVIC_SetPriority(ccu4_0_SR2_IRQN, 1U);
  // NVIC_EnableIRQ(ccu4_0_SR2_IRQN);

  NVIC_SetPriority(uart_RECEIVE_BUFFER_STANDARD_EVENT_IRQN, 2U);
  NVIC_EnableIRQ(uart_RECEIVE_BUFFER_STANDARD_EVENT_IRQN);

  setFrequency(0x0064,50);
  setPeriodCount(5);

  startTimers();

  for (;;) {
    XMC_WDT_Service();
  }
}

/* [] END OF FILE */
