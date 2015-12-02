#include "CPU20.h"

Serial pc(PTB2, PTB1);

PwmOut command_led(PTB11);

DigitalOut button_led(PTA7);
DigitalIn button_switch(PTB10);

Timer timeout_timer;

class CPU : public AnyCPU {
public:
  char snd_buf[4];
  char rcv_buf[8];
  void init(void) {
	i2c_sda.mode(PullUp);
	i2c_scl.mode(PullUp);

    slave.frequency(I2C_FREQ);
    slave.address(DEV_ADDR);
  }
  void pull(void) {
	  int i = slave.receive();

    switch (i) {
      case I2CSlave::ReadAddressed:
        char* sptr = snd_buf;
        pack_float(sptr, motor1.get_position());
        pack_float(sptr, motor1.get_velocity());
        slave.write(snd_buf, 8);
        break;
    }
  }
  void step(void) {
  }
  void push(void) {
    int o = slave.receive();
    switch (o) {
      case I2CSlave::WriteAddressed:
    	bool any_failed = false;
    	any_failed |= slave.read(rcv_buf, 4);
        char* rptr = rcv_buf;
        float result = unpack_float(rptr);
        motor1.set_command_torque(result);
        if (result < 0) {
        	result = -result;
        }
        if (result > 1) {
        	result = 1;
        }
        command_led = 1 - (result * result);

        if (!any_failed) {
        	timeout_timer.reset();
        }

        break;
    }
  }
};

int main() {
  pc.baud(115200);
  pc.puts(__FILE__ " built " __DATE__ " " __TIME__ "\r\n");

  timeout_timer.start();
  Timer alive_timer;
  alive_timer.start();

  command_led.period_ms(1);
  command_led = 0.5;

  CPU cpu;
  cpu.init();
  while (true) {
    cpu.pull();
    cpu.push();
    if (alive_timer.read_ms() > 250) {
    	alive_timer.reset();
    	button_led = !button_led;
    }
    if (timeout_timer.read_ms() > TIMEOUT_MS) {
    	motor1.timeout();
    }
	motor1.update();
  }
}
