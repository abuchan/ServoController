#include "CPU16.h"
#include "SOB.h"

Serial pc(PTB2, PTB1);

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
  pc.baud(115200);
  pc.printf(__FILE__ " built " __DATE__ " " __TIME__ "\r\n");

  CPU cpu;
  cpu.init();
  while (true) {
    cpu.pull();
    i2c_success_led = cpu.push();

    dbg_led = !dbg_led;

    pc.printf("M0: P: %i, V: %i  --  M1: P: %i V: %i\r\n",
    		(int)motor0.position_, (int)motor0.velocity_,
			(int)motor1.position_, (int)motor1.velocity_);

    wait(0.1);
    motor0.command_torque_ += 0.05;
    if (motor0.command_torque_ > 1) {
    	motor0.command_torque_ = 0;
    }
  }
}
