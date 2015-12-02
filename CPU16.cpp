#include "CPU16.h"
#include "SOB.h"

#define MODSERIAL_DEFAULT_RX_BUFFER_SIZE 32
#define MODSERIAL_DEFAULT_TX_BUFFER_SIZE 64

#include "MODSERIAL.h"

#define __ARMCC_VERSION
#include "telemetry/server-cpp/telemetry-mbed.h"

Timer main_timer;

telemetry::MbedHal telemetry_hal;
telemetry::Telemetry telemetry_obj(telemetry_hal);

telemetry::Numeric<uint32_t> time_ms(telemetry_obj,
  "time", "Time", "ms", 0);

telemetry::Numeric<float> spring(telemetry_obj,
  "spring", "Spring Constant", "?", 0.1);

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

//Serial pc(PTB2, PTB1);
//uint32_t time_ms;
//float m0_pos, m0_vel, m0_cmd_torque, m1_pos, m1_vel, m1_cmd_torque;

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

void impedance_control(float th1, float d_th1, float th2, float d_th2,
		float* t_out_1, float* t_out_2){
    // Input is motor position, velocity

    float L = 0.12; // Arm Length
    float k = spring; // Spring stiffness
    float b = 0.0; // damping
    float xp [2]; // Spring rest point
    xp[0] = L; xp[1] = L; // Setpoint is 90/90 reference position. TODO: calibrate encoders, check direction of rotation on motors

    th1 = th1 + 1.578f;

    float x [2]; // Current end-effector position
    x[0] = L*(cosf(th1) + cosf(th2));
    x[1] = L*(sinf(th1) + sinf(th2));

    float v [2]; // End-effector velocity
    v[0] = L*(-d_th1*sinf(th1)-d_th2*sinf(th2));
    v[1] = L*(d_th1*cosf(th1)+d_th2*cosf(th2));

    float f [2]; // Force
    f[0] = -k*(x[0]-xp[0]) - b*v[0];
    f[1] = -k*(x[1]-xp[1]) - b*v[1];
    // Torque calculation
    float det = 1/(L*sinf(th1+th2));

    *t_out_1 = det*(-cosf(th2)*f[0] + cosf(th1)*f[1]);
    *t_out_2 = det*(sinf(th2)*f[0] + sinf(th1)*f[1]);
}

int main() {
  main_timer.start();

  Timer alive_timer;
  alive_timer.start();

  CPU cpu;
  cpu.init();

  // have to late-create MODSERIAL, otherwise RX breaks
  MODSERIAL pc(PTB2, PTB1);
  pc.baud(115200);
  pc.puts(__FILE__ " built " __DATE__ " " __TIME__ "\r\n");

  telemetry_hal.set_serial(pc);
  telemetry_obj.transmit_header();

  while (true) {
	if (alive_timer.read_ms() > 125) {
	   	alive_timer.reset();
	 	button_led = !button_led;
	}

	master.frequency(I2C_FREQ);

    cpu.pull();

    time_ms = main_timer.read_ms();

    float t_out_1, t_out_2;
    impedance_control(motor0.get_position(), motor0.get_velocity(),
    		motor1.get_position(), motor1.get_velocity(),
			&t_out_1, &t_out_2);

    m0_cmd_torque = t_out_1;
    motor0.set_command_torque(m0_cmd_torque);
    m1_cmd_torque = t_out_2;
    motor1.set_command_torque(m1_cmd_torque);

    i2c_success_led = !cpu.push();

    m0_pos = motor0.get_position();
    m0_vel = motor0.get_velocity();
    m1_pos = motor1.get_position();
    m1_vel = motor1.get_velocity();

    spring = (float)spring;  // force update telemetry

    telemetry_obj.do_io();
  }
}
