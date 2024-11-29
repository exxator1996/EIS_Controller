#ifndef UART_CONTROL_H
#define UART_CONTROL_H

#include "modes_states_pins.h"
#include "uart_control.h"
#include <XMC1100.h>
#include <stdint.h>

typedef enum {
  DUTY_CYCLE_MODE_LR   = 0xff,
  DUTY_CYCLE_MODE_RL   = 0xfe,
  DUTY_CYCLE_MODE_BP   = 0xfd,
  DUTY_CYCLE_MODE_IDLE = 0xfc,

} dutyCycleModeCode_t;

typedef enum {
  DUTY_CYCLE_LOWER_BOUNDARY = 0,
  DUTY_CYCLE_UPPER_BOUNDARY = 100,
} dutyCycleBoundary_t;

typedef struct {} frequencyControl_s;

void stopStimulation(const mode_t *mode, uint16_t *state);

void modeSwitch(dutyCycleModeCode_t modeControlCode, mode_t *mode, const uint16_t *state);

void frequencySwitch(frequencyControl_s frequencyControlCode, uint8_t dutyCycle, const mode_t *mode);

void uartCommandEvaluation(uint16_t frequency, uint8_t dutyCycle, uint8_t periodCount, mode_t *mode, uint16_t *state);

#endif