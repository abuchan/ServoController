#include "CurrentSense.h"

CurrentSense::CurrentSense(PinName current_pin): current_pin_(current_pin) {}

float CurrentSense::get_current(void) {
    return ADC_TO_CURRENT_SCALE * current_pin_.read();
}