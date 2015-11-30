//Ben Katz, 2013
//PID Controller class

#include "mbed.h"

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PIDController {
    public:
        PIDController(float p_gain, float d_gain, float i_gain);

        float command_position(float current_position);
        float command_torque(float current_current);
        float command_position_tm(float current_position, float current_current);

        // Process variable commanded value
        float command;
        float current_torque;

        void set_command(float command);
        void set_kp(float kp) { kp_ = kp; }
        void set_kd(float kd) { kd_ = kd; }
        void set_ki(float ki) { ki_ = ki; }
        float get_kp() { return kp_; }
        float get_kd() { return kd_; }
        float get_ki() { return ki_; }

    private:
        float kp_;
        float kd_;
        float ki_;

        // State for process variable (position for now)
        float error;
        float old_error;
        float integral_error;

        // Current command (fuse with command?)
        float torque_command;
        float torque_error;
        float torque_integral_error;
        float torque_history[5];

        float direction;
};

#endif
