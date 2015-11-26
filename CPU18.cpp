#include "CPU18.h"

DigitalOut dbg_led(PTB10);

class CPU : public AnyCPU {
public:
  char snd_buf[4];
  char rcv_buf[8];
  void init(void) {
    slave.frequency(I2C_FREQ);
    slave.address(DEV_ADDR);
  }
  void pull(void) {
    int i = slave.receive();
    switch (i) {
      case I2CSlave::ReadAddressed:
        char* sptr = snd_buf;
        pack_float(sptr, motor0.get_position());
        pack_float(sptr, motor0.get_velocity());
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
        slave.read(rcv_buf, 4);
        char* rptr = rcv_buf;
        motor0.set_command_torque(unpack_float(rptr));
        dbg_led = !dbg_led;
        break;
    }
  }
};

int main() {
  CPU cpu;
  cpu.init();
  while (true) {
    cpu.pull();
    cpu.push();
  }
}
