#include "CPU16.h"
#include "SOB.h"
#include "MODSERIAL.h"

#define __ARMCC_VERSION
#include "telemetry/server-cpp/telemetry-mbed.h"

#define PID_POS_PERIOD 0.005
#define PID_POS_P_GAIN  0.5
#define PID_POS_D_GAIN  10.0
#define PID_POS_I_GAIN  0.1
PIDController position_controller(
    PID_POS_P_GAIN, PID_POS_D_GAIN, PID_POS_I_GAIN
);
Ticker position_ticker;


Timer main_timer;
MODSERIAL pc(PTB2, PTB1);

telemetry::MbedHal telemetry_hal(pc);
telemetry::Telemetry telemetry_obj(telemetry_hal);

telemetry::Numeric<uint32_t> time_ms(telemetry_obj,
  "time", "Time", "ms", 0);
telemetry::Numeric<float> m0_setpoint(telemetry_obj,
  "m0_setpoint", "Motor 0 Setpoint", "?pos", 0);
telemetry::Numeric<float> m0_pos_kp(telemetry_obj,
  "m0_kp", "Motor 0 Position Controller Kp", "??", 0);
telemetry::Numeric<float> m0_pos_kd(telemetry_obj,
  "m0_kd", "Motor 0 Position Controller Kp", "??", 0);
telemetry::Numeric<float> m0_pos_ki(telemetry_obj,
  "m0_ki", "Motor 0 Position Controller Kp", "??", 0);

telemetry::Numeric<float> m0_pos(telemetry_obj,
  "m0_pos", "Motor 0 Pos", "?pos", 0);
telemetry::Numeric<float> m0_vel(telemetry_obj,
  "m0_vel", "Motor 0 Velocity", "?pos/s", 0);
telemetry::Numeric<float> m0_cmd_torque(telemetry_obj,
  "m0_cmd_torque", "Motor 0 Commanded Torque", "?", 0);

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

//void update_position(void) {
//    m0_cmd_torque = position_controller.command_position(m0_pos);
//}

int main() {
  main_timer.start();
  pc.baud(115200);
  pc.printf(__FILE__ " built " __DATE__ " " __TIME__ "\r\n");

  CPU cpu;
  cpu.init();

  telemetry_obj.transmit_header();
//  position_ticker.attach(&update_position, PID_POS_PERIOD);
  while (true) {
    cpu.pull();
    i2c_success_led = cpu.push();

    dbg_led = !dbg_led;

    time_ms = main_timer.read_ms();
    m0_pos_kp = position_controller.get_kp();
    m0_pos_kd = position_controller.get_kd();
    m0_pos_ki = position_controller.get_ki();
    m0_pos = motor0.get_position();
    m0_vel = motor0.get_velocity();
    m0_cmd_torque = (float)m0_cmd_torque;  // force telemetry update
    m0_setpoint = (float)m0_setpoint;  // force telemetry update
    telemetry_obj.do_io();
    motor0.set_command_torque(m0_cmd_torque);
    position_controller.set_command(m0_setpoint);
    position_controller.set_kp(m0_pos_kp);
    position_controller.set_kd(m0_pos_kd);
    position_controller.set_ki(m0_pos_ki);
    m0_cmd_torque = position_controller.command_position(m0_pos);
  }
}
