#include "mbed.h"

#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

// 3.3V max ADC reading / 0.525V/A from the current amp
#define ADC_TO_CURRENT_SCALE (3.3/.525)

class CurrentSense {
    public:
        CurrentSense(PinName current_pin);
        
        float get_current(void);
    
    private:
        AnalogIn current_pin_;
};

#endif