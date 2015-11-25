#include "Encoder.h"

//Encoder steps to revolutions of output shaft in radians
float pulsesToRadians(float pulses) {
    return ((pulses/ENC_STEPS_PER_REV)*(2.0*PI));
}

//Encoder steps to revolutions of output shaft in degrees
float pulsesToDegrees(float pulses) {
    return ((pulses/ENC_STEPS_PER_REV)*360.0);
}

Encoder::Encoder(PinName enc_a_pin, PinName enc_b_pin): qei_(enc_a_pin, enc_b_pin, NC, ENC_STEPS_PER_REV, QEI::X4_ENCODING) {}

float Encoder::get_position(void) {
    return pulsesToRadians(qei_.getPulses());
}
