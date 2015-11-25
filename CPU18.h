#include "mbed.h"
#include "SOB.h"
#include "Motor.h"

#define DEV_ADDR 18
#define SDA_PIN PTB4
#define SCL_PIN PTB3
#define I2C_FREQ 100000

I2CSlave slave(SDA_PIN, SCL_PIN);
MotorSlave motor0(PTB5, PTB7, PTA5, PTA6, PTB13, PTA9);
// TODO: replace with LedButtonSlave
// LedUartButtonSlave ledbutton0(PTB1, PTB2, PTB10, PTA7);

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
        break;
    }
  }
};
