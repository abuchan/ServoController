#include "mbed.h"

#ifndef HBRIDGE_H
#define HBRIDGE_H

class HBridge {
    public:
        HBridge(PinName pwm_pin, PinName min_1_pin, PinName min_2_pin, float pwm_period);
        
        float output;
        
        void set_output(float output);
        
    private:
        PwmOut pwm_pin_;
        DigitalOut min_1_pin_;
        DigitalOut min_2_pin_;

};

#endif