#include "mbed.h"
#include "SOB.h"
#include "Motor.h"

#define DEV_ADDR 16
#define SDA_PIN PTB4
#define SCL_PIN PTB3
#define I2C_FREQ 100000

DigitalIn i2c_sda(SDA_PIN);
DigitalIn i2c_scl(SCL_PIN);
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
