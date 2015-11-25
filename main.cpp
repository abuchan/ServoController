//Ben Katz, 2013
//Austin Buchan, 2015
#include "mbed.h"
#include "Motor.h"
#include "PID.h"

/*
// Original
#define PWM_PIN PTB13
#define MIN_1_PIN PTA0
#define MIN_2_PIN PTA8
#define ENC_A_PIN PTB6
#define ENC_B_PIN PTB7
#define CURRENT_PIN PTA9
*/

// Auto generated
#define PWM_PIN PTB13
#define MIN_1_PIN PTA5
#define MIN_2_PIN PTA6
#define ENC_A_PIN PTB5
#define ENC_B_PIN PTB7
#define CURRENT_PIN PTA9

#define UART_RX_PIN PTB1
#define UART_TX_PIN PTB2

#define PWM_PERIOD 0.001
#define PID_CUR_PERIOD 0.001
#define PID_POS_PERIOD 0.005

#define PID_POS_P_GAIN  0.5
#define PID_POS_D_GAIN  10.0
#define PID_POS_I_GAIN  0.1

#define PID_CUR_P_GAIN  0.5
#define PID_CUR_D_GAIN  0.0
#define PID_CUR_I_GAIN  0.01

Serial pc(UART_TX_PIN, UART_RX_PIN);

Motor motor(
    PWM_PIN, MIN_1_PIN, MIN_2_PIN, PWM_PERIOD, 
    CURRENT_PIN,
    ENC_A_PIN, ENC_B_PIN, 
    PID_CUR_P_GAIN, PID_CUR_D_GAIN, PID_CUR_I_GAIN
);

PIDController position_controller(
    PID_POS_P_GAIN, PID_POS_D_GAIN, PID_POS_I_GAIN
);

Ticker current_ticker;

void update_current(void) {
    motor.update();
}

Ticker position_ticker;

void update_position(void) {
    motor.set_command(position_controller.command_position(motor.get_position()));
}

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

int main() {
    printf("Built " __DATE__ " " __TIME__ "\r\n");
    
    led1 = 1;
    led2 = 1;
    led3 = 1;
    
    wait(.5);
    
    led2 = 0;
    printf("Initializing test\n\r");
    
    current_ticker.attach(&update_current, PID_CUR_PERIOD);
    position_ticker.attach(&update_position, PID_POS_PERIOD);
    
    int count = 0;
    float command = 0.0;
    
    led3 = 0;
    printf("Starting\n\r");
    
    while(1){
        command = 1.0 * abs((55-count)/10) - 0.25;
        position_controller.set_command(command);
        
        led1 = (count % 2);
        
        printf("MP:%f MV:%f MCu:%f MCo:%f MO:%f MT:%f P:%f\n\r",
            motor.position, motor.velocity, motor.current, motor.command, motor.output, motor.torque, position_controller.command
        );
        
        count = (count + 1) % 100;
        
        wait(0.1);
    }
}
