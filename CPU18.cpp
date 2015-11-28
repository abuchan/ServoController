#include "CPU18.h"

Serial pc(PTB2, PTB1);

PwmOut loop_led(PTB9);
PwmOut command_led(PTA0);

class CPU : public AnyCPU {
public:
  char snd_buf[4];
  char rcv_buf[8];
  void init(void) {
    slave.frequency(I2C_FREQ);
    slave.address(DEV_ADDR);
    motor0.init();
  }
  void pull(void) {
	__disable_irq();
    int i = slave.receive();
    switch (i) {
      case I2CSlave::ReadAddressed:
        char* sptr = snd_buf;
        pack_float(sptr, motor0.get_position());
        pack_float(sptr, motor0.get_velocity());
        slave.write(snd_buf, 8);
        break;
    }
    __enable_irq();
  }
  void step(void) {
  }
  void push(void) {
    __disable_irq();
    int o = slave.receive();
    switch (o) {
      case I2CSlave::WriteAddressed:
        slave.read(rcv_buf, 4);
        char* rptr = rcv_buf;
        float result = unpack_float(rptr);
        motor0.set_command_torque(result);
        if (result > 1) {
        	result = 1;
        } else if (result < 0) {
        	result = 0;
        }
        command_led = 1 - result;
        break;
    }
    __enable_irq();
  }
};

int main() {
  pc.baud(115200);

  command_led.period_ms(1);

  CPU cpu;
  cpu.init();
  while (true) {
    cpu.pull();
    cpu.push();
    loop_led = !loop_led;
    pc.printf("%i\r\n", (int)motor0.get_velocity());
  }
}
