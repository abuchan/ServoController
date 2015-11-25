#include "mbed.h"
#include "QEI.h"

#ifndef ENCODER_H
#define ENCODER_H


#define PI  3.14159265358979323846
#define ENC_STEPS_PER_REV 1200

class Encoder {
    public:
        Encoder(PinName enc_a_pin, PinName enc_b_pin);
        
        float get_position(void);
        
    private:
        QEI qei_;

};

#endif