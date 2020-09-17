#include <Arduino.h>

#include "MAX30105.h"
#include "heartRate.h"
#include "motor.h"

#define DEBUG

#define PIN_LED 5
#define OPEN_POSITION 5
#define TAKE_POSITION 2
#define OUT_POSITION 1
TaskHandle_t taskGreenLEDHandler;
enum BOXNAME { BOX1 = 1, BOX2, BOX3, BOX4, BOX5, BOX6 };
int boxdisplacement = 0;
void taskPrint(void *pvParameters);
void taskGreenLED(void *pvParameters);
void taskServo1Test(void *pvParameters);
void rotateToOpen(int boxName);
void rotateToClose(int boxName);
void rotateToTake(int boxName);
void rotateToOut(int boxName);
void takePills(int boxName, int pillsNumber);

void setup() {
    Serial.begin(115200);
    servo1.attach(PIN_SERVO1);
    servo2.attach(PIN_SERVO2);
    servo3.attach(PIN_SERVO3);
    servo4.attach(PIN_SERVO4);
    servo5.attach(PIN_SERVO5);
    servo6.attach(PIN_SERVO6);
    pinMode(5, OUTPUT);
    // put your setup code here, to run once:
    xTaskCreate(taskPrint, "taskPrint", 1000, NULL, 1, NULL);
    xTaskCreate(taskGreenLED, "greenLED", 1000, NULL, 1, &taskGreenLEDHandler);
    xTaskCreate(taskServo1Test, "taskServo1Test", 1000, NULL, 1, NULL);
}

void loop() {
    // put your main code here, to run repeatedly:
    if (Serial.available()) {
        char message;
        message = Serial.read();
        if (message == 'c') {
            vTaskDelete(taskGreenLEDHandler);
            digitalWrite(PIN_LED, LOW);
        }
        if (message == 'o') {
            xTaskCreate(taskGreenLED, "greenLED", 1000, NULL, 1,
                        &taskGreenLEDHandler);
        }
    }
}

void taskPrint(void *pvParameters) {
    for (int i = 0; i < 10; i++) {
        Serial.println("Hello from task 1");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    Serial.println("Ending task 1");
    vTaskDelete(NULL);
}
void taskGreenLED(void *pvParameters) {
    while (1) {
        digitalWrite(PIN_LED, HIGH);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        digitalWrite(PIN_LED, LOW);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}
void taskServo1Test(void *pvParameters) {
    while (1) {
        for (int i = 20; i <= 150; i++) {
            servo1.write(i);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
        for (int i = 150; i >= 20; i--) {
            servo1.write(i);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    }
}
void rotateToOpen(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = OPEN_POSITION - currentPosition;
    if (targetRotation >= 0) {
        #ifdef DEBUG
            Serial.println("rorate right" + targetRotation);
        #endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
        }
    } else {
        #ifdef DEBUG
            Serial.println("rorate left" + (-targetRotation));
        #endif
        for(int i = 0; i < -targetRotation; i++){
            taskRotate(LEFT);
        }
    }
}
void rotateToClose(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = OPEN_POSITION - currentPosition;
    if (targetRotation >= 0) {
        #ifdef DEBUG
            Serial.println("rorate right" + targetRotation);
        #endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
        }
    } else {
        #ifdef DEBUG
            Serial.println("rorate left" + (-targetRotation));
        #endif
        for(int i = 0; i < -targetRotation; i++){
            taskRotate(LEFT);
        }
    }
}
void rotateToTake(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = TAKE_POSITION - currentPosition;
    if (targetRotation >= 0) {
        #ifdef DEBUG
            Serial.println("rorate right" + targetRotation);
        #endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
        }
    } else {
        #ifdef DEBUG
            Serial.println("rorate left" + (-targetRotation));
        #endif
        for(int i = 0; i < -targetRotation; i++){
            taskRotate(LEFT);
        }
    }
}
void rotateToOut(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = OUT_POSITION - currentPosition;
    if (targetRotation >= 0) {
        #ifdef DEBUG
            Serial.println("rorate right" + targetRotation);
        #endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
        }
    } else {
        #ifdef DEBUG
            Serial.println("rorate left" + (-targetRotation));
        #endif
        for(int i = 0; i < -targetRotation; i++){
            taskRotate(LEFT);
        }
    }
}
void takePills(int boxName, int pillsNumber) {
    rotateToOpen(boxName);
    taskOpenBox(NULL);
    rotateToTake(boxName);
    stepper2.runToNewPosition(5000);
        open_pump();
        stepper2.runToNewPosition(6400);
        // open_pump();
        delay(1000);
        digitalWrite(PIN_STEPPER2_DIR, LOW);
        for (int i = 0; i < 400; i++) {
            digitalWrite(PIN_STEPPER2_STEP, HIGH);
            delayMicroseconds(200);
            digitalWrite(PIN_STEPPER2_STEP, LOW);
            delayMicroseconds(200);
        }
        delay(500);
        digitalWrite(PIN_STEPPER2_DIR, HIGH);
        for (int i = 0; i < 400; i++) {
            digitalWrite(PIN_STEPPER2_STEP, HIGH);
            delayMicroseconds(200);
            digitalWrite(PIN_STEPPER2_STEP, LOW);
            delayMicroseconds(200);
        }

        stepper2.setMaxSpeed(2500);
        stepper2.setAcceleration(1500);
        stepper2.runToNewPosition(-20000);
        close_pump();
        delay(2000);
        // stepper2.runToNewPosition(0);
}