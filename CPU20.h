#include "mbed.h"
#include "SOB.h"
#include "Motor.h"

#define DEV_ADDR 20
#define SDA_PIN PTB4
#define SCL_PIN PTB3
#define I2C_FREQ 100000

#define TIMEOUT_MS 10

DigitalIn i2c_sda(SDA_PIN);
DigitalIn i2c_scl(SCL_PIN);

I2CSlave slave(SDA_PIN, SCL_PIN);
MotorSlave motor1(PTB5, PTB7, PTA5, PTA6, PTB13, PTA9);
// TODO: replace with LEDButtonSlave
// LedUartButtonSlave ledbutton1(PTB1, PTB2, PTB10, PTA7);
