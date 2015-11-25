#include "HBridge.h"

HBridge::HBridge(PinName pwm_pin, PinName min_1_pin, PinName min_2_pin, float pwm_period): pwm_pin_(pwm_pin), min_1_pin_(min_1_pin), min_2_pin_(min_2_pin) {
    pwm_pin_.period(pwm_period);
}

//Input of -1 means full reverse, 1 means full forward
//An input of magnitude > 1 gets reduced to 1 (or -1) by the pwm.write function
void HBridge::set_output(float output) {
    this->output = output;
    if (output > 0) {
        min_1_pin_.write(0);
        min_2_pin_.write(1);
        pwm_pin_.write(output);
    } else if (output < 0) {
        min_1_pin_.write(1);
        min_2_pin_.write(0);
        pwm_pin_.write(output * -1);
    } else {
        min_1_pin_.write(0);
        min_2_pin_.write(0);
        pwm_pin_.write(0.0);
    }
}