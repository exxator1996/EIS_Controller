#ifndef UART_CONTROL_H
#define UART_CONTROL_H

#include "modes_states_pins.h"
#include <XMC1100.h>
#include <stdint.h>

void stopStimulation(const mode_t *const mode, uint16_t *const state);

void modeSwitch(const uint8_t *const modeControlCode, mode_t *const mode);

void frequencySwitch(const uint16_t *const frequencyControlCode, const uint8_t *const dutyCycle,
                     const mode_t *const mode);

void uartCommandEvaluation(const uint16_t *const frequency, const uint8_t *const dutyCycle,
                           const uint8_t *const periodCount, mode_t *const mode, uint16_t *const state);

#endif