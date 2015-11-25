#include "mbed.h"
#include "SOB.h"
#include "Motor.h"

#define DEV_ADDR 16
#define SDA_PIN PTB4
#define SCL_PIN PTB3
#define I2C_FREQ 100000

I2C master(SDA_PIN, SCL_PIN);
// TODO: instantiate UART
// UartHeaderMaster uart;
MotorMaster motor0;
MotorMaster motor1;
// TODO: replace with LEDButton
// LedButtonMaster ledbutton_master;
// TODO: replace with LEDButtonSlave
// LedUartButtonMaster ledbutton0;
//LedUartButtonMaster ledbutton1;

class CPU : public AnyCPU {
public:
  char snd_buf[4];
  char rcv_buf[8];
  void init(void) {
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
  void push(void) {
    char* sptr;
    sptr = snd_buf;
    pack_float(sptr, motor0.command_torque_);
    master.write(18, snd_buf, 4);
    sptr = snd_buf;
    pack_float(sptr, motor1.command_torque_);
    master.write(20, snd_buf, 4);
  }
};
