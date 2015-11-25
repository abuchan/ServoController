#include "mbed.h"
#include "QEI.h"
#include "Encoder.h"
#include "HBridge.h"
#include "CurrentSense.h"
#include "PID.h"

#ifndef MOTOR_H
#define MOTOR_H

class Motor {
    public:
        Motor(
            PinName pwm_pin, PinName min_1_pin, PinName min_2_pin, float pwm_period,
            PinName current_pin,
            PinName enc_a_pin, PinName enc_b_pin, 
            float pid_cur_p_gain, float pid_cur_d_gain, float pid_cur_i_gain
        );
        
        float position;
        float velocity;
        float current;
        float torque;
        float command;
        
        // output to PWM
        float output;
        
        // Update all state variables and apply command
        void update(void);
        
        float get_position(void);
        float get_velocity(void);
        float get_current(void);
        
        // Set current controller commanded value
        void set_command(float command);
    
    private:
        Timer timer_;
        HBridge hbridge_;
        CurrentSense current_sense_;
        Encoder encoder_;
        PIDController current_controller_;
        
};
        
#endif