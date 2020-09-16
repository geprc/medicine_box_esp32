#include "motor.h"

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
AccelStepper stepper1(1, STEPPER1_STEP, STEPPER1_DIR);
AccelStepper stepper2(1, STEPPER2_STEP, STEPPER2_DIR);
AccelStepper stepper3(1, STEPPER3_STEP, STEPPER3_DIR);
AccelStepper stepper4(1, STEPPER4_STEP, STEPPER4_DIR);

int pal, step;

void motor_init(void) {
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, HIGH);  //拉高关闭电机
    pinMode(STEPPER1_EN, OUTPUT);
    pinMode(STEPPER2_EN, OUTPUT);
    pinMode(STEPPER3_EN, OUTPUT);
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo3.attach(SERVO3_PIN);
    servo4.attach(SERVO4_PIN);
    servo5.attach(SERVO5_PIN);
    servo6.attach(SERVO6_PIN);
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
    stepper4.setMaxSpeed(10000);
    stepper4.setAcceleration(8000);
    digitalWrite(STEPPER2_DIR, HIGH);
    while (!digitalRead(8)) {
        digitalWrite(STEPPER2_STEP, HIGH);
        delayMicroseconds(200);
        digitalWrite(STEPPER2_STEP, LOW);
        delayMicroseconds(200);
    }
}

void open_pump(void) { digitalWrite(PUMP_PIN, LOW); }
void close_pump(void) { digitalWrite(PUMP_PIN, HIGH); }

void rotate(int direction) {
    int j;
    digitalWrite(STEPPER1_DIR, direction == RIGHT ? HIGH : LOW);
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
    //     digitalWrite(STEPPER1_STEP, HIGH);
    //     delayMicroseconds(abs(i-2050)*1000/2050+300);
    // }
    for (j = 0; j < 3200; j++) {
        digitalWrite(STEPPER1_STEP, LOW);
        digitalWrite(STEPPER1_STEP, HIGH);
        delayMicroseconds(1400);
    }
}

void open_box(void) {
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

void take(int num) {
    int i;
    for (i = 0; i < num; i++) {
        stepper2.runToNewPosition(5000);
        open_pump();
        stepper2.runToNewPosition(6400);
        // open_pump();
        delay(1000);
        digitalWrite(STEPPER2_DIR, LOW);
        for (int i = 0; i < 400; i++) {
            digitalWrite(STEPPER2_STEP, HIGH);
            delayMicroseconds(200);
            digitalWrite(STEPPER2_STEP, LOW);
            delayMicroseconds(200);
        }
        delay(500);
        digitalWrite(STEPPER2_DIR, HIGH);
        for (int i = 0; i < 400; i++) {
            digitalWrite(STEPPER2_STEP, HIGH);
            delayMicroseconds(200);
            digitalWrite(STEPPER2_STEP, LOW);
            delayMicroseconds(200);
        }

        stepper2.setMaxSpeed(2500);
        stepper2.setAcceleration(1500);
        stepper2.runToNewPosition(-20000);
        close_pump();
        delay(2000);
        // stepper2.runToNewPosition(0);
    }
}

void close_box(void) {
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

void add_medicine(void) {
    stepper4.runToNewPosition(64000);  //预设步数400
    delay(3000);
    stepper4.runToNewPosition(0);
}

void flyblock(void) {
    int i;
    for (i = 0; i < 180; i++) {
        servo6.write(i);
        delay(10);
    }
}

void out(void) {
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

// void rotate(int selected_box){
// 	int i,j;
// 	long time = -0.039 * j * j + 1.25 * j + 300;
// 	step = selected_box * 3200;
// 	while(step > 19200){
// 		step -= 19200;
// 	}
// 	while(step < 0){
// 		step += 19200;
// 	}
// 		pal =  step - 9600;

// 	if(pal>0) {
// 		digitalWrite(STEPPER1_DIR, 0);
// 		for(i=0;i<pal/3200;i++){
// 			for(j=0;j<1000;j++){
// 				digitalWrite(STEPPER1_STEP, LOW);
// 				digitalWrite(STEPPER1_STEP, HIGH);
// 				delayMicroseconds(abs(i-2050)*1000/2050+300);
// 			}
// 			for(j=1000;j<2200;j++){
// 				digitalWrite(STEPPER1_STEP, LOW);
// 				digitalWrite(STEPPER1_STEP, HIGH);
// 				delayMicroseconds(1400);
// 			}
// 			for(j=2200;j<3200;j++){
// 				digitalWrite(STEPPER1_STEP, LOW);
// 				digitalWrite(STEPPER1_STEP, HIGH);
// 				delayMicroseconds(abs(i-2050)*1000/2050+300);
// 			}
// 	}
// 	}
// 	else{
// 		digitalWrite(STEPPER1_DIR, 1);
// 		pal = 9600 - step;
// 		for(i=0;i<pal/3200;i++){
// 			for(j=0;j<1000;j++){
// 				digitalWrite(STEPPER1_STEP, LOW);
// 				digitalWrite(STEPPER1_STEP, HIGH);
// 				delayMicroseconds(abs(i-2050)*1000/2050+300);
// 			}
// 			for(j=1000;j<2200;j++){
// 				digitalWrite(STEPPER1_STEP, LOW);
// 				digitalWrite(STEPPER1_STEP, HIGH);
// 				delayMicroseconds(1400);
// 			}
// 			for(j=2200;j<3200;j++){
// 				digitalWrite(STEPPER1_STEP, LOW);
// 				digitalWrite(STEPPER1_STEP, HIGH);
// 				delayMicroseconds(abs(i-2050)*1000/2050+300);
// 			}

// 		}
// 	}
// }