#include "mbed.h"
#include "QEI.h"
#include "Encoder.h"
#include "HBridge.h"
#include "CurrentSense.h"
#include "PID.h"

#ifndef MOTOR_H
#define MOTOR_H

// TODO: remote set interfaces
#define PWM_PERIOD 0.001
#define PID_CUR_PERIOD 0.001

#define PID_CUR_P_GAIN  0.5
#define PID_CUR_D_GAIN  0.0
#define PID_CUR_I_GAIN  0.01

// Interface specification for Motor.
class MotorInterface {
public:
    virtual ~MotorInterface() {};

    virtual float get_position(void) = 0;
    virtual float get_velocity(void) = 0;
    virtual float get_current(void) = 0;
    virtual float get_torque(void) = 0;

    // Set current controller commanded value
    virtual void set_command(float command) = 0;
};

// Struct of motor data to return every cycle on an update.
struct MotorData {
    float position;
	float velocity;
	float current;
	float torque;
};

// Actual Motor class, to be instantiated on the local processor which the motor
// is attached to.
// TODO: separate this from MotorSlave wrapper which provides default constants
// and provides stateful layer over position / velocity.
class Motor {
public:
	Motor(PinName quad_a_pin, PinName quad_b_pin, PinName current_pin,
			PinName h_bridge_pwm_pin, PinName h_bridge_in1_pin,
			PinName h_bridge_in2_pin,
			float pwm_period,
			float current_p_gain, float current_d_gain, float current_i_gain) :
		position_(0),
		encoder_(quad_a_pin, quad_b_pin),
		hbridge_(h_bridge_pwm_pin, h_bridge_in1_pin, h_bridge_in2_pin, pwm_period),
		current_sense_(current_pin),
		current_controller_(current_p_gain, current_d_gain, current_i_gain) {
	}

	// Run control loop and return new state variables.
	MotorData update(float command);

private:
	// Last position, for sampling velocity
	float position_;

	Encoder encoder_;
	HBridge hbridge_;
	CurrentSense current_sense_;
	PIDController current_controller_;

	Timer timer_;
};

// Master slave class, wrapper around a Motor for convenience.
class MotorSlave : MotorInterface {
	MotorSlave(PinName quad_a_pin, PinName quad_b_pin, PinName current_pin,
			PinName h_bridge_pwm_pin, PinName h_bridge_in1_pin,
			PinName h_bridge_in2_pin) :
		motor_(quad_a_pin, quad_b_pin, current_pin,
				h_bridge_pwm_pin, h_bridge_in1_pin, h_bridge_in2_pin,
				PWM_PERIOD, PID_CUR_P_GAIN, PID_CUR_D_GAIN, PID_CUR_I_GAIN),
		position_(0), velocity_(0), current_(0), torque_(0), command_(0) {}

    // Update all state variables and apply command
	void init(void);
    void update(void);

    float get_position(void) { return position_; }
    float get_velocity(void) { return velocity_; }
    float get_torque(void) { return torque_; }
    float get_current(void) { return current_; }

    // Set current controller commanded value
    void set_command(float command) { command_ = command; }

private:
    Ticker ticker_;
    Motor motor_;

    float position_;
    float velocity_;
    float current_;
    float torque_;

    float command_;
};

// Master stub class, does nothing but communicate.
class MotorMaster : MotorInterface {
	// TODO: proper encapsulation of state variables
    float position_;
    float velocity_;
    float current_;
    float torque_;
    float command_;

    // Update all state variables and apply command
    void update(void);

    float get_position(void) { return position_; }
    float get_velocity(void) { return velocity_; }
    float get_torque(void) { return torque_; }
    float get_current(void) { return current_; }

    // Set current controller commanded value
    virtual void set_command(float command) { command_ = command; }
};

#endif
