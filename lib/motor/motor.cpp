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
    servo6.write(0);
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
    Serial.println("正在归位");
    digitalWrite(PIN_STEPPER2_DIR, HIGH);
    while(!digitalRead(PIN_CLICK)) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(300);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(300);
    }
    Serial.println("已归位");
}
void midToLeft() {
    //TODO
    Serial.println("中到左...");
    digitalWrite(PIN_STEPPER2_DIR, HIGH);
    for(int i=0; i<=5000; i++) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delay(2);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delay(2);
    }
}
void leftToMid() {
    //TODO
    Serial.println("左到中...");
    digitalWrite(PIN_STEPPER2_DIR, LOW);
    for(int i=0; i<=5000; i++) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(200);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(200);
    }
}
void taskMidToRight(void *pvParameters) {
    //TODO
    Serial.println("中到右...");
    digitalWrite(PIN_STEPPER2_DIR, LOW);
    for(int i=0; i<=5000; i++) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(200);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(200);
    }
}
void taskRightToMid(void *pvParameters) {
    //TODO
    Serial.println("右到中...");
    digitalWrite(PIN_STEPPER2_DIR, HIGH);
    for(int i=0; i<=5000; i++) {
        digitalWrite(PIN_STEPPER2_STEP, HIGH);
        delayMicroseconds(200);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(200);
    }
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
    for (int i = 0; i < 1600; i++) {
        digitalWrite(PIN_STEPPER1_STEP, LOW);
        vTaskDelay(3 / portTICK_PERIOD_MS);
        digitalWrite(PIN_STEPPER1_STEP, HIGH);
        // delayMicroseconds(1400);
        vTaskDelay(3 / portTICK_PERIOD_MS);
    }
    Serial.println("已旋转一步");
}

//换药步进电机推出
void taskPushBoxOut(void *pvParameters) {
    //TODO
    digitalWrite(PIN_STEPPER3_DIR, HIGH);
    for(int i=0; i<=10000; i++) {
        digitalWrite(PIN_STEPPER3_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_STEPPER3_STEP, LOW);
        delay(1);
    }
}
//换药步进电机拉入
void taskPullBoxIn(void *pvParameters) {
    //TODO
    digitalWrite(PIN_STEPPER3_DIR, LOW);
    for(int i=0; i<=10000; i++) {
        digitalWrite(PIN_STEPPER3_STEP, HIGH);
        delay(1);
        digitalWrite(PIN_STEPPER3_STEP, LOW);
        delay(1);
    }
}

void taskOpenBox(void *pvParameters) {
    //TODO
    Serial.println("openbox...ing");
    int i;
    for (i = 50; i < 120; i++) {
        servo4.write(170 - i);
        servo5.write(170 - i);
        delay(20);
    }
    delay(500);
    for (i = 0; i <= 180; i++) {
        servo6.write(i);
        delay(20);
    }
    delay(500);
    servo6.write(180);
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
    delay(500);
    for (i = 180; i >= 0; i--) {
        servo6.write(i);
        delay(20);
    }
    delay(500);
    servo6.write(0);
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
    Serial.println("pills out");
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
    Serial.println("open pump");
    digitalWrite(PIN_PUMP, LOW);
}
void closePump() {
    Serial.println("close pump");
    digitalWrite(PIN_PUMP, HIGH);
}