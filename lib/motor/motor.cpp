#include <motor.h>

Servo servo1;                                                   //小翻台左
Servo servo2;                                                   //小翻台右
Servo servo3;                                                   //小翻台中
Servo servo4;                                                   //开盖舵机左
Servo servo5;                                                   //开盖舵机右
Servo servo6;                                                   //开盖舵机
AccelStepper stepper1(1, PIN_STEPPER1_STEP, PIN_STEPPER1_DIR);  //整体托盘步进电机
AccelStepper stepper2(1, PIN_STEPPER2_STEP, PIN_STEPPER2_DIR);  //取药机构步进电机
AccelStepper stepper3(1, PIN_STEPPER3_STEP, PIN_STEPPER3_DIR);  //药盒加药步进电机

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
    for (int i = 50; i < 120; i++) {
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
    stepper2Return();
}
//取药步进电机回位
void stepper2Return() {
    digitalWrite(PIN_STEPPER2_DIR, HIGH);
    while(digitalRead(PIN_CLICK)) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(100);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(100);
    }
    digitalWrite(PIN_STEPPER2_DIR, LOW);
    while (!digitalRead(PIN_CLICK)) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(200);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(200);
    }
}
void taskMidToLeft(void *pvParameters) {
    //TODO
}
void taskLeftToMid(void *pvParameters) {
    //TODO
}
void taskMidToRight(void *pvParameters) {
    //TODO
}
void taskRightToMid(void *pvParameters) {
    //TODO
}

void taskRotate(int direction) {
    // int direction = (int)pvParameters;
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

//换药步进电机推出
void taskPushBoxOut(void *pvParameters) {
    //TODO
    stepper3.runToNewPosition(64000);
}
//换药步进电机拉入
void taskPullBoxIn(void *pvParameters) {
    //TODO
    stepper3.runToNewPosition(0);
}

void taskOpenBox(void *pvParameters) {
    //TODO
    int i;
    for (i = 50; i < 120; i++) {
        servo4.write(170 - i);
        servo5.write(170 - i);
        delay(20);
    }
    servo6.write(180);
    delay(600);
    servo6.write(91);
    for (i = 50; i < 120; i++) {
        servo4.write(i);
        servo5.write(i);
        delay(20);
    }
}
void taskCloseBox(void *pvParameters) {
    //TODO
    int i;
    for (i = 50; i < 120; i++) {
        servo4.write(170 - i);
        servo5.write(170 - i);
        delay(20);
    }
    servo6.write(0);
    delay(800);
    servo6.write(91);
    for (i = 50; i < 120; i++) {
        servo4.write(i);
        servo5.write(i);
        delay(20);
    }
}

//翻台舵机
void taskPillsUp(void *pvParameters) {
    //TODO
    servo3.write(25);
}
void taskPillsDown(void *pvParameters) {
    //TODO
    servo3.write(60);
}

//出药翻板
void taskPillsOut(void *pvParameters) {
    //TODO
    int i;
    for (i = 7; i <= 140; i++) {
        servo1.write(i);
        servo2.write(170 - i);
        delay(10);
    }
    delay(3000);
    for (i = 140; i >= 7; i--) {
        servo1.write(i);
        servo2.write(170 - i);
        delay(10);
    }
}

void openPump() {
    digitalWrite(PIN_PUMP, LOW);
}
void closePump() {
    digitalWrite(PIN_PUMP, HIGH);
}