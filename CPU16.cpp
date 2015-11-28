#include "CPU16.h"
#include "SOB.h"
#include "MODSERIAL.h"

#define __ARMCC_VERSION
#include "telemetry/server-cpp/telemetry-mbed.h"

Timer main_timer;
MODSERIAL pc(PTB2, PTB1);

telemetry::MbedHal telemetry_hal(pc);
telemetry::Telemetry telemetry_obj(telemetry_hal);

telemetry::Numeric<uint32_t> time_ms(telemetry_obj,
  "time", "Time", "ms", 0);
telemetry::Numeric<float> motor0_pos(telemetry_obj,
  "motor0_pos", "Motor 0 Pos", "?", 0);
telemetry::Numeric<float> motor0_vel(telemetry_obj,
  "motor0_vel", "Motor 0 Velocity", "?/s", 0);
telemetry::Numeric<float> motor0_cmd_torque(telemetry_obj,
  "motor0_cmd_torque", "Motor 0 Commanded Torque", "s", 0);

DigitalOut dbg_led(PTB10);

DigitalOut i2c_success_led(PTB8);

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

  CPU cpu;
  cpu.init();

  telemetry_obj.transmit_header();

  while (true) {
    cpu.pull();
    i2c_success_led = cpu.push();

    dbg_led = !dbg_led;

    time_ms = main_timer.read_ms();
    motor0_pos = motor0.get_position();
    motor0_vel = motor0.get_velocity();
    motor0_cmd_torque = (float)motor0_cmd_torque;  // force telemetry update
    telemetry_obj.do_io();
    motor0.set_command_torque(motor0_cmd_torque);

    wait(0.01);
  }
}
