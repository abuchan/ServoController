#include "Motor.h"

// All variables updated here so sampling rates are constant.
MotorData Motor::update(float torque_command) {
	MotorData data_out;

	// Sample the encoder and timer as close together as possible for accuracy
	float timer_elapsed = timer_.read();
	float new_position = encoder_.get_position();
	timer_.reset();

	data_out.velocity = (new_position - position_) / timer_elapsed;
	position_ = new_position;
	data_out.position = position_;
	data_out.current = current_sense_.get_current();

    current_controller_.set_command(torque_command);
    float output = current_controller_.command_torque(data_out.current);
    data_out.torque = current_controller_.current_torque;
    hbridge_.set_output(output);

    return data_out;
}


void MotorSlave::init(void) {
	ticker_.attach(this, &MotorSlave::update, PID_CUR_PERIOD);
}

void MotorSlave::update(void) {
	MotorData data = motor_.update(command_torque_);
	position_ = data.position;
	velocity_ = data.velocity;
	current_ = data.current;
	torque_ = data.torque;
}
