#include <Arduino.h>

#include "MAX30105.h"
#include "heartRate.h"
#include "motor.h"

#define PIN_LED 5
TaskHandle_t taskGreenLEDHandler;

void taskPrint(void *pvParameters);
void taskGreenLED(void *pvParameters);
void taskServoTest(void *pvParameters);
void taskMidtoLeft(void *pvParameters);
void taskMidtoRight(void *pvParameters);

void setup() {
    Serial.begin(115200);
    servo1.attach(PIN_SERVO1);
    pinMode(5, OUTPUT);
    // put your setup code here, to run once:
    xTaskCreate(taskPrint, "taskPrint", 1000, NULL, 1, NULL);
    xTaskCreate(taskGreenLED, "greenLED", 1000, NULL, 1, &taskGreenLEDHandler);
    xTaskCreate(taskServoTest, "taskServoTest", 1000, NULL, 1, NULL);
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
void taskServoTest(void *pvParameters) {
    while (1) {
        servo1.write(90);
        Serial.println("servo 90 deg");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        servo1.write(0);
        Serial.println("servo  0 deg");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}