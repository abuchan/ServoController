#include "CPU16.h"
#include "SOB.h"
#include "MODSERIAL.h"

#define __ARMCC_VERSION
#include "telemetry/server-cpp/telemetry-mbed.h"

#define PID_POS_PERIOD 0.005
#define PID_POS_P_GAIN  0.5
#define PID_POS_D_GAIN  10.0
#define PID_POS_I_GAIN  0
PIDController m0_pid(
    PID_POS_P_GAIN, PID_POS_D_GAIN, PID_POS_I_GAIN
);
PIDController m1_pid(
    PID_POS_P_GAIN, PID_POS_D_GAIN, PID_POS_I_GAIN
);

Timer main_timer;
MODSERIAL pc(PTB2, PTB1);

telemetry::MbedHal telemetry_hal(pc);
telemetry::Telemetry telemetry_obj(telemetry_hal);

telemetry::Numeric<uint32_t> time_ms(telemetry_obj,
  "time", "Time", "ms", 0);

telemetry::Numeric<float> m0_pos(telemetry_obj,
  "m0_pos", "", "?pos", 0);
telemetry::Numeric<float> m0_vel(telemetry_obj,
  "m0_vel", "", "?pos/s", 0);
telemetry::Numeric<float> m0_cmd_torque(telemetry_obj,
  "m0_cmd_torque", "", "?", 0);

telemetry::Numeric<float> m1_pos(telemetry_obj,
  "m1_pos", "", "?pos", 0);
telemetry::Numeric<float> m1_vel(telemetry_obj,
  "m1_vel", "", "?pos/s", 0);
telemetry::Numeric<float> m1_cmd_torque(telemetry_obj,
  "m1_cmd_torque", "", "?", 0);

DigitalOut i2c_success_led(PTB11);

DigitalOut button_led(PTB7);
DigitalIn button_switch(PTB1);

class CPU : public AnyCPU{
public:
  char snd_buf[4];
  char rcv_buf[8];
  void init(void) {
	i2c_sda.mode(PullUp);
	i2c_scl.mode(PullUp);

    master.frequency(I2C_FREQ);
  }
  void pull(void) {
    char* rptr;
    master.read(18, rcv_buf, 8);
    rptr = rcv_buf;
    motor0.position_ = unpack_float(rptr);
    motor0.velocity_ = unpack_float(rptr);
    master.read(20, rcv_buf, 8);
    rptr = rcv_buf;
    motor1.position_ = unpack_float(rptr);
    motor1.velocity_ = unpack_float(rptr);
  }
  bool push(void) {
	bool any_success = false;

    char* sptr;
    int result;
    sptr = snd_buf;
    pack_float(sptr, motor0.command_torque_);
    result = (master.write(18, snd_buf, 4) != 0);
    any_success |= (result == 0);
    sptr = snd_buf;
    pack_float(sptr, motor1.command_torque_);
    result = (master.write(20, snd_buf, 4) != 0);
    any_success |= (result == 0);

    return any_success;
  }
};


int main() {
  main_timer.start();
  pc.baud(115200);
  pc.printf(__FILE__ " built " __DATE__ " " __TIME__ "\r\n");

  Timer alive_timer;
  alive_timer.start();
  Timer telemetry_timer;
  telemetry_timer.start();

  CPU cpu;
  cpu.init();

  telemetry_obj.transmit_header();

  while (true) {
	if (alive_timer.read_ms() > 125) {
	   	alive_timer.reset();
	 	button_led = !button_led;
	}

	master.frequency(I2C_FREQ);

    cpu.pull();

    time_ms = main_timer.read_ms();

    m0_cmd_torque = m0_pid.command_position(motor0.get_position());
    motor0.set_command_torque(m0_cmd_torque);
    m1_cmd_torque = m1_pid.command_position(motor1.get_position());
    motor1.set_command_torque(m1_cmd_torque);

    i2c_success_led = !cpu.push();

    m0_pos = motor0.get_position();
    m0_vel = motor0.get_velocity();
    m1_pos = motor1.get_position();
    m1_vel = motor1.get_velocity();

    telemetry_obj.do_io();
    telemetry_timer.reset();

    // Hacky way to allow a single telemetry variable to have a different
    // meaning for remote set.
    if (m0_pos != motor0.get_position()) {
        m0_pid.set_command(m0_pos);
        pc.printf("M0RX\n");
    }
    if (m1_pos != motor1.get_position()) {
        m1_pid.set_command(m1_pos);
        pc.printf("M1RX\n");
    }
  }
}
