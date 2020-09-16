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
#define PIN_PUMP             5 //气泵控制
#define PIN_CLICK           18 //取药步进电机复位
#define PIN_Y               23 //Y
#define PIN_Z               19 //Z
#define RIGHT             0x00 //分药步进电机顺时针旋转
#define LEFT              0x01 //分药步进电机逆时针旋转
Servo servo1;                                                   //小翻台左
Servo servo2;                                                   //小翻台右
Servo servo3;                                                   //小翻台中
Servo servo4;                                                   //开盖舵机左
Servo servo5;                                                   //开盖舵机右
Servo servo6;                                                   //开盖舵机
AccelStepper stepper1(1, PIN_STEPPER1_STEP, PIN_STEPPER1_DIR);  //整体托盘步进电机
AccelStepper stepper2(1, PIN_STEPPER2_STEP, PIN_STEPPER2_DIR);  //取药机构步进电机
AccelStepper stepper3(1, PIN_STEPPER3_STEP, PIN_STEPPER3_DIR);  //药盒加药步进电机

void motor_init(void);
void stepper1_reset(void);
void open_pump(void);
void close_pump(void);
void go_open(int b);
void rotate(int direction);
void open_box(void);
void take(int num);
void close_box(void);
void go_close(void);
void mode(void);
void out(void);
void add_medicine(void);
void flyblock(void);

/**
 * 步进电机控制函数（多线程）
*/
//取药步进电机
void stepper2Return();
void taskMidToLeft(void *pvParameters);
void taskLeftToMid(void *pvParameters);
void taskMidToRight(void *pvParameters);
void taskRightToMid(void *pvParameters);

//分药步进电机旋转
void taskRotate(void *pvParameters);

//换药步进电机
void taskOut(void *pvParameters);
void taskIn(void *pvParameters);

#endif