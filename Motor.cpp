#include "Motor.h"

Motor::Motor(
    PinName pwm_pin, PinName min_1_pin, PinName min_2_pin, float pwm_period,
    PinName current_pin,
    PinName enc_a_pin, PinName enc_b_pin, 
    float pid_cur_p_gain, float pid_cur_d_gain, float pid_cur_i_gain) : 
    hbridge_(pwm_pin, min_1_pin, min_2_pin, pwm_period), 
    current_sense_(current_pin), 
    encoder_(enc_a_pin, enc_b_pin), 
    current_controller_(pid_cur_p_gain, pid_cur_d_gain, pid_cur_i_gain) {
    
    timer_.start();
}

void Motor::update(void) {
    this->velocity = this->get_velocity();
    this->current = this->get_current();
    current_controller_.set_command(this->command);
    this->output = current_controller_.command_torque(this->current);
    this->torque = current_controller_.current_torque;
    this->hbridge_.set_output(this->output);
}

float Motor::get_position(void) {
    return encoder_.get_position();
}

// Implicitly updates position
float Motor::get_velocity(void) {
    float old_position = this->position;
    this->position = this->get_position();
    float velocity = (this->position - old_position)/timer_.read();
    timer_.reset();
    return velocity;
}

float Motor::get_current(void) {
    return current_sense_.get_current();
}

void Motor::set_command(float command) {
    this->command = command;
}