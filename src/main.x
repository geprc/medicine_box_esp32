#include <Arduino.h>
#include <Wire.h>
#include "motor.h"
#include <MS5611.h>

MS5611 ms5611;

double referencePressure;

bool isRotate, isOpen = true;
char bluetooth_read;
int8_t angle1 = 7, angle2 = 163;
int32_t pill_in_box[6] = {999, 999, 999, 999, 999, 999};

String pill_name[6] = {"1", "2", "3", "4", "5", "6"};

void (* resetFunc) (void) = 0;
void open_air() {
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
}
void close_air() {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
}

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    motor_init();
    close_air();
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    servo3.write(0);
    delay(500);
    servo3.write(20);
    // for (size_t i = 0; i < 3; i++) {
    //     rotate(RIGHT);
    // }
    // open_box();
    // for (size_t i = 0; i < 2; i++) {
    //     rotate(LEFT);
    // }
    // stepper2.runToNewPosition(5000);
    // open_pump();
    // stepper2.runToNewPosition(7400);
    // delay(500);
    // while(!Serial1.available()){
    //     digitalWrite(STEPPER2_DIR, LOW);
    //     for (int i = 0; i < 600; i++) {
    //         digitalWrite(STEPPER2_STEP, HIGH);
    //         delayMicroseconds(200);
    //         digitalWrite(STEPPER2_STEP, LOW);
    //         delayMicroseconds(200);
    //     }
    //     delay(300);
    //     digitalWrite(STEPPER2_DIR, HIGH);
    //     for (int i = 0; i < 600; i++) {
    //         digitalWrite(STEPPER2_STEP, HIGH);
    //         delayMicroseconds(200);
    //         digitalWrite(STEPPER2_STEP, LOW);
    //         delayMicroseconds(200);
    //     }
    // }
    // stepper2.setMaxSpeed(2000);
    // stepper2.setAcceleration(1500);
    // stepper2.runToNewPosition(-20000);
    // close_pump();
    // delay(5000);
    // out();
    // for (size_t i = 0; i < 1; i++)
    // {
    //     rotate(LEFT);
    // }
}

void loop() {
    if (Serial1.available()) {
        bluetooth_read = Serial1.read();
        switch (bluetooth_read) {
        case 'a':
            angle1 += 1;
            servo4.write(angle1);
            Serial1.println("angle1:" + String(angle1));
            break;
        case 'b':
            angle1 -= 1;
            servo4.write(angle1);
            Serial1.println("angle1:" + String(angle1));
            break;
        case 'c':
            angle2 += 1;
            servo5.write(angle1);
            Serial1.println("angle2:" + String(angle2));
            break;
        case 'd':
            angle2 -= 1;
            servo5.write(angle1);
            Serial1.println("angle2:" + String(angle2));
            break;
        case 'e':
            //取药
            stepper2.runToNewPosition(5000);
            close_air();
            open_pump();
            stepper2.runToNewPosition(5600);
            delay(500);
            // while(!Serial1.available()){
                digitalWrite(STEPPER2_DIR, LOW);
                for (int i = 0; i < 600; i++) {
                    digitalWrite(STEPPER2_STEP, HIGH);
                    delayMicroseconds(400);
                    digitalWrite(STEPPER2_STEP, LOW);
                    delayMicroseconds(400);
                }
                delay(300);
                digitalWrite(STEPPER2_DIR, HIGH);
                for (int i = 0; i < 600; i++) {
                    digitalWrite(STEPPER2_STEP, HIGH);
                    delayMicroseconds(400);
                    digitalWrite(STEPPER2_STEP, LOW);
                    delayMicroseconds(400);
                }
            // }
            stepper2.setMaxSpeed(2000);
            stepper2.setAcceleration(1500);
	        stepper2.runToNewPosition(-20000);
	        close_pump();
            open_air();
            delay(2000);
            close_air();
            break;
        case 'g':
            //左旋
            rotate(LEFT);
            break;
        case 'h':
            //右旋
            rotate(RIGHT);
            break;
        case 'i':
            //开盖
            open_box();
            break;
        case 'j':
            //关盖
            close_box();
            break;
        case 'k':
            //出药
            out();
            break;
        case 'l':
            //换药
            add_medicine();
            break;
        case 'm':
            open_air();
            break;
        case 'n':
            close_air();
            break;
        case 'o':
            servo3.write(25);
            break;
        case 'p':
            servo3.write(60);
            break;
        default:
            break;
        }
    }
}