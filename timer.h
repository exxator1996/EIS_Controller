#ifndef TIMER_H
#define TIMER_H

#include "modes_states_pins.h"
#include <XMC1100.h>
#include <math.h>


void setPeriodTime(const double_t period, const uint8_t* const dutyCycle);

void setFrequency(const double_t frequency, const uint8_t* const dutyCycle, const mode_t* const mode);

void setPeriodCount(const uint8_t* const periodCountValue,const mode_t* const mode);

#endif
