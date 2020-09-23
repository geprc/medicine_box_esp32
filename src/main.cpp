#include <Arduino.h>

#include "MAX30105.h"
#include "heartRate.h"
#include "motor.h"

#define DEBUG

#define OUT_POSITION 1
#define TAKE_POSITION 2
#define OPEN_POSITION 5

bool isTaskTake = false;
int boxdisplacement = 0;
int pillsParameters[2] = {3, 1};
enum BOXNAME { BOX1 = 1, BOX2, BOX3, BOX4, BOX5, BOX6 };
TaskHandle_t taskGreenLEDHandler;

void rotateToOpen(int boxName);
void rotateToClose(int boxName);
void rotateToTake(int boxName);
void rotateToOut(int boxName);
void takePills(int boxName, int pillsNumber);
void taskTakePills(void *pvParameters);

void setup() {
    Serial.begin(115200);
    delay(1000);
    motor_init();
    // xTaskCreate(taskPrint, "taskPrint", 1000, NULL, 1, NULL);
}

void loop() {
    if (Serial.available()) {
        char message = ' ';
        message = Serial.read();
        if (message == 't') {
            if (!isTaskTake) {
                isTaskTake = true;
                Serial.println("*************\n*开始取药过程*\n*************");
                // xTaskCreate(taskTakePills, "taskTakePills", 20000,
                            // pillsParameters, 1, NULL);
                takePills(3, 1);
            } else {
                Serial.println("已经有一个取药任务了");
            }
        }
        if (message == 'i') {//开盖
            if (!isTaskTake) {
                isTaskTake = true;
                taskOpenBox(NULL);
                isTaskTake = false;
            } else {
                Serial.println("已经有一个取药任务了");
            }
        }
        if (message == 'j') {//关盖
            if (!isTaskTake) {
                isTaskTake = true;
                taskCloseBox(NULL);
                digitalWrite(PIN_ENABLE, HIGH);
                isTaskTake = false;
            } else {
                Serial.println("已经有一个取药任务了");
            }
        }
        if (message == 'k') {
            if (!isTaskTake) {
                isTaskTake = true;
                Serial.println("开始推出药盒");
                digitalWrite(PIN_ENABLE, LOW);
                taskPushBoxOut(NULL);
                delay(1000);
                Serial.println("开始推入药盒");
                taskPullBoxIn(NULL);
                digitalWrite(PIN_ENABLE, HIGH);
                isTaskTake = false;
            } else {
                Serial.println("已经有一个取药任务了");
            }
        }
        if (message == '3') {
            taskPillsOut(NULL);
        }
        if (message == 'a') {//开气泵
            openPump();
        }
        if (message == 'b') {//关气泵
            closePump();
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
    digitalWrite(PIN_ENABLE, LOW);
    // taskPillsUp(NULL);
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
        delayMicroseconds(400);
        digitalWrite(PIN_STEPPER2_STEP, LOW);
        delayMicroseconds(400);
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
    delay(5000);
    // xTaskCreate(taskRightToMid, "taskRightToMid", 1000, NULL, 1, NULL);
    taskPillsOut(NULL);
    isTaskTake = false;
    digitalWrite(PIN_ENABLE, HIGH);
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