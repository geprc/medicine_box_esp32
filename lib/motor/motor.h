#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Servo.h>
#include <AccelStepper.h>

#define PIN_SERVO1          32 //小翻台舵机左
#define PIN_SERVO2          33 //小翻台舵机右
#define PIN_SERVO3          25 //小翻台舵机中
#define PIN_SERVO4          26 //开盖舵机左
#define PIN_SERVO5          27 //开盖舵机左
#define PIN_SERVO6          14 //开盖舵机中
#define PIN_STEPPER1_DIR    13 //整体托盘步进电机方向
#define PIN_STEPPER1_STEP   12 //整体托盘步进电机控制
#define PIN_STEPPER2_DIR    15 //取药机构步进电机方向
#define PIN_STEPPER2_STEP    2 //取药机构步进电机控制
#define PIN_STEPPER3_DIR     4 //药盒加药步进电机方向
#define PIN_STEPPER3_STEP    0 //药盒加药步进电机控制
#define PIN_ENABLE           5 //步进电机enable
#define PIN_CLICK           18 //取药步进电机复位
#define PIN_PUMP            23 //气泵控制
#define PIN_AIR             19 //通气电磁阀
#define RIGHT             0x00 //分药步进电机顺时针旋转
#define LEFT              0x01 //分药步进电机逆时针旋转
#define ANGLE_UP            85 //翻板上角度
#define ANGLE_DOWN         110 //翻板下角度
extern Servo servo1;           //小翻台左
extern Servo servo2;           //小翻台右
extern Servo servo3;           //小翻台中
extern Servo servo4;           //开盖舵机左
extern Servo servo5;           //开盖舵机右
extern Servo servo6;           //开盖舵机
extern AccelStepper stepper1;  //整体托盘步进电机
extern AccelStepper stepper2;  //取药机构步进电机
extern AccelStepper stepper3;  //药盒加药步进电机

void motor_init(void);
/**
 * 步进电机控制函数（多线程）
*/
//取药步进电机
void stepper2Return();
void midToLeft();
void leftToMid();
void taskMidToRight(void *pvParameters);
void taskRightToMid(void *pvParameters);

//分药步进电机旋转
// void taskRotate(void *pvParameters);
void taskRotate(int direction);

//换药步进电机
void taskPushBoxOut(void *pvParameters);
void taskPullBoxIn(void *pvParameters);

/**
 * 舵机控制函数
 * 
 */
//开盖模块舵机
void taskOpenBox(void *pvParameters);
void taskCloseBox(void *pvParameters);

//翻台舵机
void taskPillsUp(void *pvParameters);
void taskPillsDown(void *pvParameters);

//出药翻板
void taskPillsOut(void *pvParameters);

void openPump();
void closePump();

void openAir();
void closeAir();

#endif