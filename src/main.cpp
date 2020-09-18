#include <Arduino.h>

#include "MAX30105.h"
#include "heartRate.h"
#include "motor.h"

#define DEBUG

#define PIN_LED 5
#define OUT_POSITION 1
#define TAKE_POSITION 2
#define OPEN_POSITION 4

bool isTaskLED = false;
bool isTaskTake = false;
int boxdisplacement = 0;
int pillsParameters[2] = {1, 1};
enum BOXNAME { BOX1 = 1, BOX2, BOX3, BOX4, BOX5, BOX6 };
TaskHandle_t taskGreenLEDHandler;

void taskSerial(void *pvParameters);
void taskPrint(void *pvParameters);
void taskGreenLED(void *pvParameters);
void taskServo1Test(void *pvParameters);
void rotateToOpen(int boxName);
void rotateToClose(int boxName);
void rotateToTake(int boxName);
void rotateToOut(int boxName);
void takePills(int boxName, int pillsNumber);
void taskTakePills(void *pvParameters);

void setup() {
    Serial.begin(115200);
    delay(1000);
    pinMode(PIN_LED, OUTPUT);
    // xTaskCreate(taskPrint, "taskPrint", 1000, NULL, 1, NULL);
}

void loop() {
    if (Serial.available()) {
        char message = ' ';
        message = Serial.read();
        if (message == 'c') {
            if (isTaskLED) {
                vTaskDelete(taskGreenLEDHandler);
                isTaskLED = false;
                digitalWrite(PIN_LED, LOW);
            } else {
                Serial.println("已经没有LED任务了");
            }
        }
        if (message == 'o') {
            if (!isTaskLED) {
                xTaskCreate(taskGreenLED, "greenLED", 1000, NULL, 1,
                            &taskGreenLEDHandler);
                isTaskLED = true;
            } else {
                Serial.println("已经有一个LED任务了");
            }
        }
        if (message == 't') {
            if (!isTaskTake) {
                isTaskTake = true;
                Serial.println("开始取药过程");
                xTaskCreate(taskTakePills, "taskTakePills", 10000,
                pillsParameters,
                            1, NULL);
                // Serial.println("开始推出药盒");
                // taskPushBoxOut(NULL);
                // Serial.println("开始推入药盒");
                // taskPullBoxIn(NULL);
            } else {
                Serial.println("已经有一个取药任务了");
            }
        }
    }
}

void taskPrint(void *pvParameters) {
    while (1) {
        Serial.println("Hello from task 1");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void taskGreenLED(void *pvParameters) {
    while (1) {
        digitalWrite(PIN_LED, HIGH);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        digitalWrite(PIN_LED, LOW);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
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
        Serial.println("rorate right " + String(targetRotation) +
                       " step to open.");
        Serial.println("向右旋转 " + String(targetRotation) + " 步以开盖.");
#endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
            boxdisplacement++;
        }
    } else {
#ifdef DEBUG
        Serial.println("rorate left " + String(-targetRotation) +
                       " step to open.");
        Serial.println("向左旋转 " + String(-targetRotation) + " 步以开盖.");
#endif
        for (int i = 0; i < -targetRotation; i++) {
            taskRotate(LEFT);
            boxdisplacement--;
        }
    }
}
void rotateToClose(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = OPEN_POSITION - currentPosition;
    if (targetRotation >= 0) {
#ifdef DEBUG
        Serial.println("rorate right " + String(targetRotation) +
                       " step to close.");
        Serial.println("向右旋转 " + String(targetRotation) + " 步以关盖.");
#endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
            boxdisplacement++;
        }
    } else {
#ifdef DEBUG
        Serial.println("rorate right " + String(-targetRotation) +
                       " step to close.");
        Serial.println("向左旋转 " + String(-targetRotation) + " 步以关盖.");
#endif
        for (int i = 0; i < -targetRotation; i++) {
            taskRotate(LEFT);
            boxdisplacement--;
        }
    }
}
void rotateToTake(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = TAKE_POSITION - currentPosition;
    if (targetRotation >= 0) {
#ifdef DEBUG
        Serial.println("rorate right " + String(targetRotation) +
                       " step to take pills.");
        Serial.println("向右旋转 " + String(targetRotation) + " 步以取药.");
#endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
            boxdisplacement++;
        }
    } else {
#ifdef DEBUG
        Serial.println("rorate right " + String(-targetRotation) +
                       " step to take pills.");
        Serial.println("向左旋转 " + String(-targetRotation) + " 步以取药.");
#endif
        for (int i = 0; i < -targetRotation; i++) {
            taskRotate(LEFT);
            boxdisplacement--;
        }
    }
}
void rotateToOut(int boxName) {
    int currentPosition = boxName + boxdisplacement;
    int targetRotation = OUT_POSITION - currentPosition;
    if (targetRotation >= 0) {
#ifdef DEBUG
        Serial.println("rorate right " + String(targetRotation) +
                       " step to add pills.");
        Serial.println("向右旋转 " + String(targetRotation) + " 步以换(加)药.");
#endif
        for (int i = 0; i < targetRotation; i++) {
            taskRotate(RIGHT);
            boxdisplacement++;
        }
    } else {
#ifdef DEBUG
        Serial.println("rorate right " + String(-targetRotation) +
                       " step to add pills.");
        Serial.println("向左旋转 " + String(-targetRotation) +
                       " 步以换(加)药.");
#endif
        for (int i = 0; i < -targetRotation; i++) {
            taskRotate(LEFT);
            boxdisplacement--;
        }
    }
}
void takePills(int boxName, int pillsNumber) {
    taskPillsUp(NULL);
    rotateToOpen(boxName);
    taskOpenBox(NULL);
    rotateToTake(boxName);
    // stepper2.runToNewPosition(5000);
    midToLeft();
    openPump();
    // stepper2.runToNewPosition(6400);
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
#ifdef DEBUG
    Serial.println("吸到啦！");
#endif
    // stepper2.setMaxSpeed(2500);
    // stepper2.setAcceleration(1500);
    // stepper2.runToNewPosition(-20000);
    leftToMid();
    taskMidToRight(NULL);
    closePump();
    delay(2000);
    taskPillsOut(NULL);
    isTaskTake = false;
}

void taskTakePills(void *pvParameters) {
    int boxName = ((int *)pvParameters)[0];
    int pillsNumber = ((int *)pvParameters)[1];
#ifdef DEBUG
    Serial.println("boxName: " + String(boxName));
    Serial.println("pillsNumber: " + String(pillsNumber));
#endif
    takePills(boxName, pillsNumber);
    vTaskDelete(NULL);
}