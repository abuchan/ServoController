#include "CPU16.h"
int main() {
  printf("Built " __DATE__ " " __TIME__ "\r\n");

  CPU cpu;
  cpu.init();
  while (true) {
    cpu.pull();

    printf("M0: P: %f, V: %f  M1: P: %f V: %f",
    		motor0.position_, motor0.velocity_, motor1.position_, motor1.velocity_);

    cpu.push();
  }
}
