#include "timer.h"
#include "modes_states_pins.h"
#include "cycfg_peripherals.h"
#include "xmc_ccu4.h"
#include <math.h>
#include <XMC1100.h>
// Prescaler Value
double_t timerTickTimes[] = {1.5625E-8, 3.125e-8, 6.25e-8, 1.25e-7, 2.5e-7, 5e-7,    1e-6,    2e-6,
                             4e-6,      8e-6,     1.6e-5,  3.2e-5,  6.4e-5, 1.28e-4, 2.56e-4, 5.12e-4};

XMC_CCU4_SLICE_PRESCALER_t prescalerValues[] = {
    XMC_CCU4_SLICE_PRESCALER_1,    XMC_CCU4_SLICE_PRESCALER_2,    XMC_CCU4_SLICE_PRESCALER_4,
    XMC_CCU4_SLICE_PRESCALER_8,    XMC_CCU4_SLICE_PRESCALER_16,   XMC_CCU4_SLICE_PRESCALER_32,
    XMC_CCU4_SLICE_PRESCALER_64,   XMC_CCU4_SLICE_PRESCALER_128,  XMC_CCU4_SLICE_PRESCALER_256,
    XMC_CCU4_SLICE_PRESCALER_512,  XMC_CCU4_SLICE_PRESCALER_1024, XMC_CCU4_SLICE_PRESCALER_2048,
    XMC_CCU4_SLICE_PRESCALER_4096, XMC_CCU4_SLICE_PRESCALER_8192, XMC_CCU4_SLICE_PRESCALER_16384,
    XMC_CCU4_SLICE_PRESCALER_32768};

void setPeriodTime(double_t const period, const uint8_t* const dutyCycle) {
  // Prescaler stoppen um ihn einstellen zu können
  XMC_CCU4_StopPrescaler(ccu4_0_HW);
  uint32_t ticksPeriod  = 0;
  uint32_t ticksCompare = 0;

  // Niedrigst möglichen Prescaler Wert ermitteln der eine vollständige Periode laufen kann
  for (int i = 0; i < 16; i++) {
    if ((timerTickTimes[i] * 0xFFFF) >= period) {
      XMC_CCU4_SLICE_SetPrescaler(timerFreq_HW, prescalerValues[i]);
      // timer ticks period = periodeValue in s / time between timer ticks
      ticksPeriod  = (uint32_t)round(period / timerTickTimes[i]) - 1;
      // timer ticks compare output on for duty cycle in %
      ticksCompare = (uint32_t)round(ticksPeriod * ((100 - *dutyCycle) / 100.0));
      break;
    }
  }

  XMC_CCU4_StartPrescaler(ccu4_0_HW);

  XMC_CCU4_SLICE_SetTimerCompareMatch(timerFreq_HW, ticksCompare);
  XMC_CCU4_SLICE_SetTimerPeriodMatch(timerFreq_HW, ticksPeriod);

  XMC_CCU4_EnableShadowTransfer(ccu4_0_HW, (XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_SLICE_1));
  XMC_CCU4_SLICE_StartTimer(timerFreq_HW);
}

void setFrequency(const double_t frequency, const uint8_t* const dutyCycle, const mode_t* const mode) {
  // Nur Frequenzen zwischen 100 mHz und 10 kHz
  if (frequency >= 0.1 && frequency <= 10000) {
    // Frequenz verdoppeln weil hier vier statt zwei Zustandswechsel stattfinden müssen
    if (*mode == MODE_BP)
      setPeriodTime(1.0 / (frequency * 2.0), dutyCycle);
    else
      setPeriodTime(1.0 / frequency, dutyCycle);
  }
}

void setPeriodCount(const uint8_t* const periodCountValue, const mode_t* const mode) {
  XMC_CCU4_SLICE_StopClearTimer(timerPeriodCount_HW);
  if (*mode == MODE_RL || *mode == MODE_LR) {
    XMC_CCU4_SLICE_SetTimerPeriodMatch(timerPeriodCount_HW, (2 * (*periodCountValue)));
  } else if (*mode == MODE_BP) {
    XMC_CCU4_SLICE_SetTimerPeriodMatch(timerPeriodCount_HW, (4 * (*periodCountValue)));
  }
  XMC_CCU4_EnableShadowTransfer(ccu4_0_HW, XMC_CCU4_SHADOW_TRANSFER_SLICE_2);
  XMC_CCU4_SLICE_StartTimer(timerPeriodCount_HW);
}
