#include <motor.h>

void motor_init(void) {
    pinMode(PIN_PUMP, OUTPUT);
    digitalWrite(PIN_PUMP, HIGH);  //拉高关闭气泵电机
    // pinMode(STEPPER1_EN),OUTPUT);
    // pinMode(STEPPER2_EN,OUTPUT);
    // pinMode(STEPPER3_EN,OUTPUT);
    // TODO 步进电机使能
    servo1.attach(PIN_SERVO1);
    servo2.attach(PIN_SERVO2);
    servo3.attach(PIN_SERVO3);
    servo4.attach(PIN_SERVO4);
    servo5.attach(PIN_SERVO5);
    servo6.attach(PIN_SERVO6);
    servo1.write(7);
    servo2.write(163);
    servo3.write(25);
    int i;
    for (i = 50; i < 120; i++) {
        servo4.write(i);
        servo5.write(i);
        delay(20);
    }
    servo4.write(120);
    servo5.write(120);
    servo6.write(91);
    stepper1.setMaxSpeed(1000);
    stepper1.setAcceleration(500);
    stepper2.setMaxSpeed(1500);
    stepper2.setAcceleration(1500);
    stepper3.setMaxSpeed(500);
    stepper3.setAcceleration(100);
    digitalWrite(PIN_STEPPER2_DIR, HIGH);
    while (!digitalRead(8)) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(200);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(200);
    }
}
void taskMidToLeft(void *pvParameters) {

}
void taskLeftToMid(void *pvParameters) {
    
}
void taskMidToRight(void *pvParameters) {}
void taskRightToMid(void *pvParameters) {}

void taskRotate(void *pvParameters) {
    int direction = (int)pvParameters;
    digitalWrite(PIN_STEPPER1_DIR, direction == RIGHT ? HIGH : LOW);
    // for(j=0;j<1000;j++){
    //     digitalWrite(STEPPER1_STEP, LOW);
    //     digitalWrite(STEPPER1_STEP, HIGH);
    //     delayMicroseconds(abs(i-2050)*1000/2050+300);
    // }
    // for(j=1000;j<2200;j++){
    //     digitalWrite(STEPPER1_STEP, LOW);
    //     digitalWrite(STEPPER1_STEP, HIGH);
    //     delayMicroseconds(1400);
    // }
    // for(j=2200;j<3200;j++){
    //     digitalWrite(STEPPER1_STEP, LOW);
    //     digitalWrite(STEPPER1_STEP, HIGH);i
    //     delayMicroseconds(abs(i-2050)*100i0/i2050+300);
    // }
    //两分频
    for (int i = 0; i < 400; i++) {
        digitalWrite(PIN_STEPPER1_STEP, LOW);
        vTaskDelay(5 / portTICK_PERIOD_MS);
        digitalWrite(PIN_STEPPER1_STEP, HIGH);
        // delayMicroseconds(1400);
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}