#ifndef UART_CONTROL_H
#define UART_CONTROL_H

#include "modes_states_pins.h"
#include <XMC1100.h>
#include <stdint.h>

void stopStimulation(const mode_t *const mode, uint16_t *const state);

void modeSwitch(const uint8_t modeControlCode, mode_t *const mode, const uint16_t* const state);

void frequencySwitch(const uint16_t frequencyControlCode, const uint8_t dutyCycle,
                     const mode_t* const mode);

void uartCommandEvaluation(const uint16_t frequency, const uint8_t dutyCycle,
                           const uint8_t periodCount, mode_t *const mode, uint16_t *const state);

#endif