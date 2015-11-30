#include "PID.h"

PIDController::PIDController(float p_gain, float d_gain, float i_gain) {
    kp_ = p_gain;
    kd_ = d_gain;
    ki_ = i_gain;

    command = 0.0;
    error = 0.0;
    old_error = 0.0;
    integral_error = 0.0;
    
    current_torque = 0.0;
    torque_command = 0.0;
    torque_error = 0.0;
    torque_integral_error = 0.0;
    
    direction = 0;
}

void PIDController::set_command(float command) {
    this->command = command;
    this->torque_command = command;
    this->integral_error = 0.0;
    this->torque_integral_error = 0.0;
}

//voltage mode position control
//This function is called to set the desired position of the servo
float PIDController::command_position(float current_position) {
    this->error = (this->command - current_position);
    this->integral_error += this->error;
    if (this->integral_error > 1.0)
        this->integral_error = 1.0;
    else if (this->integral_error < -1.0)
        this->integral_error = -1.0;
    float output = (this->kp_)*(this->error) + (this->kd_)*(this->error - this->old_error) + (this->ki_)*(this->integral_error);
    this->old_error = this->error;
    return output;
}

// Torque Mode position control
float PIDController::command_position_tm(float current_position, float current_current) {
    float output = this->command_position(current_position);
    output += this->command_torque(current_current);
    return output;
}

// Given a current current in abs(Amps), produce a PWM command between -1.0 (full reverse) and 1.0 (full forward)
float PIDController::command_torque(float current_current) {
    if (this->direction != 0){
        this->current_torque = this->direction*(current_current);
    } else {
        this->current_torque = current_current;
    }
    
    float average_torque = 0;
   
    for (int i = 0; i < 4; i++){
        this->torque_history[i] = this->torque_history[i+1];
        average_torque += this->torque_history[i];
    }
    average_torque += current_torque;
    average_torque = average_torque/5;
    //average_torque = current_torque; // Does this just overwrite the average?
    this->torque_history[4] = current_torque;
    
    this->torque_error = (this->torque_command - average_torque);

    this->torque_integral_error += this->torque_error;
    
    float output = 4.0*this->torque_command + this->kp_*this->torque_error + this->ki_*this->torque_integral_error;
    
    // Set direction based on output
    if (output > 0){
        this->direction = 1;
    } else if(output < 0){
        this->direction = -1;
    } else{
        output = 0;
    }
   
    // Coerce ouptut to be between -1 and 1
    if (output > 1){
        output = 1;
    } else if (output < -1){
        output = -1;
    }
    
    return output;
}