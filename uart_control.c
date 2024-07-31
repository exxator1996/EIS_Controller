#include "uart_control.h"
#include "cycfg_peripherals.h"
#include "modes_states_pins.h"
#include "timer.h"
#include "xmc_ccu4.h"
#include <XMC1100.h>
#include <stdint.h>
#include <xmc_scu.h>

void stopStimulation(const mode_t *const mode, uint16_t *const state) {
  XMC_CCU4_SLICE_StopClearTimer(timerFreq_HW);
  // Sichereren Zustand herstellen
  PORT0->OMR = lookupMatrix[*mode][0];
  *state     = 0;
}

void modeSwitch(const uint8_t *const modeControlCode, mode_t *const mode, uint16_t *const state) {
  switch (*modeControlCode) {
  case 0xFF:
    *mode = MODE_LR;
    break;
  case 0xFE:
    *mode = MODE_RL;
    break;
  case 0xFD:
    *mode = MODE_BP;
    break;
  case 0xFC:
  // Wenn keine gültige eingabe automatisch in sicheren IDLE Zustand
  default:
    *mode = MODE_IDLE;
    break;
  }
  //Ausgang in abhängikeit von neuem Zustand setzen
  PORT0->OMR = lookupMatrix[*mode][*state];
}

void frequencySwitch(const uint16_t *const frequencyControlCode, const uint8_t *const dutyCycle,
                     const mode_t *const mode) {
  // Kodierte werte für Frequenzen mit Dezimalstelle
  switch (*frequencyControlCode) {
  case 0xFFFF:
    setFrequency(0.1, dutyCycle, mode);
    break;
  case 0xFFFE:
    setFrequency(0.4, dutyCycle, mode);
    break;
  case 0xFFFD:
    setFrequency(0.8, dutyCycle, mode);
    break;
  default:
    setFrequency(*frequencyControlCode, dutyCycle, mode);
    break;
  }
}

void uartCommandEvaluation(const uint16_t *const frequency, const uint8_t *const dutyCycle,
                           const uint8_t *const periodCount, mode_t *const mode, uint16_t *const state) {
  // Wenn die Frequenz 0 empfangen wird wird die Anregung sofort gestoppt
  // Ansonsten wird geprüft ob über den Tastgrad eine kodierte Anweisung (FF, FE, FD oder FC) zum Moduswechsel
  // übertragen wird Wenn ein Modus wechsel durchgeführt wird zunächst die Anregung gestoppt und dann der Startzustand
  // des Modus auf den Port ausgegeben
  //! Nach einem Moduswechsel sollte mindestens 30 ms gewartet werden, damit die Tracos sich initialisieren können
  // Alles Null == Master Reset
  if (*frequency == 0 && *dutyCycle == 0 && *periodCount == 0) {
    XMC_SCU_RESET_AssertMasterReset();
  } else if (*frequency == 0) {
    stopStimulation(mode, state);
  } else if (*dutyCycle <= 0xFF && *dutyCycle >= 0xFC) {
    // Modus Wechsel
    stopStimulation(mode, state);
    modeSwitch(dutyCycle, mode, state);
  } else if (*dutyCycle >= 0 && *dutyCycle <= 100) {
    // Perioden Zähler setzen (0 = Keine begrezung der Periodenzahl)
    if (*periodCount != 0) {
      setPeriodCount(periodCount, mode);
    }

    // Frequenz einstellen
    frequencySwitch(frequency, dutyCycle, mode);
  }
}