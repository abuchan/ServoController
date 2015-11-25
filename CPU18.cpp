#include "CPU18.h"
int main() {
  CPU cpu;
  cpu.init();
  while (true) {
    cpu.pull();
    cpu.push();
  }
}
