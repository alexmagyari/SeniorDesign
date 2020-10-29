/*
 * motor.h
 *
 *  Created on: Sep 30, 2020
 *      Author: Alexander Magyari
 */


#define MOTOR_PWM_PERIOD (251 - 1)
#define MAX_MAIN_MOTOR_PWM (int)(MOTOR_PWM_PERIOD * 3 / 4) - 1
#define MIN_MAIN_MOTOR_PWM (MOTOR_PWM_PERIOD - MAX_MAIN_MOTOR_PWM) + 1
#define MAX_MOTOR_OFFSET (MOTOR_PWM_PERIOD - MAX_MAIN_MOTOR_PWM - 1)


// Initialize PWM module by setting timer and
// linking pins to respect timer capture/control
void initPWM(int period, int startDuty);

// Adjust the main motor PWM duty cycle which controls
// all four of the motors
void editMainPWM(int duty);

// Adjust motor PWM offset
void editMotorPWM(int motor, int duty);

// Run the motors through their arming sequence for start
void armMotors(void);

// Disable motor function
void disarmMotors(void);
