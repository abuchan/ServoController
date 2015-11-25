#ifndef __IS_SOB__
#define __IS_SOB__

class AnyCPU {
 public:
  uint8_t unpack_uint8 (char*& ptr) {
    uint8_t res = *((uint8_t*)ptr);
    ptr += sizeof(char);
    return res;
  }

  float unpack_float (char*& ptr) {
    float res = *((float*)ptr);
    ptr += sizeof(float);
    return res;
  }

  void pack_uint8 (char*& ptr, uint8_t x) {
    *((uint8_t*)ptr) = x;
    ptr += sizeof(uint8_t);
  }

  void pack_float (char*& ptr, float x) {
    *((float*)ptr) = x;
    ptr += sizeof(float);
  }

  void init ( void ) { }
  void step ( void ) { }
  void pull ( void ) { }
  void push ( void ) { }
  void dispatch ( void ) { 
    for (;;) {
      pull();
      step();
      push();
    }
  }
  // #include "CPU16.h"
  // void CPU::step ( void ) {
  //    // user code goes here
  // }
  // int main (int argc, char* argv[]) {
  //   CPU cpu;
  //   cpu.dispatch();
  // }
};

class AnyPeripheral {
 public:
  void init ( void ) { }
  void step ( void ) { }
};

#endif
