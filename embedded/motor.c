#include <ti/devices/msp432p4xx/inc/msp.h>
#include <motor.h>
/*
 * motor.c
 *
 *  Created on: Sep 30, 2020
 *      Author: Alexander Magyari
 */


void initPWM(int period, int startDuty){
    if (startDuty > 100)
        startDuty = 100;
        if (startDuty < 0)
            startDuty = 0;
        startDuty = startDuty * MOTOR_PWM_PERIOD / 100;

    /*** GPIO Setup***/
    P7DIR |= BIT4;              // P1.6 as output
    P7DIR |= BIT5;              // P1.6 as output
    P7DIR |= BIT6;              // P1.6 as output
    P7DIR |= BIT7;              // P1.6 as output

    P7SEL0 |= BIT4;             // P1.6 as Timer0_B Out1
    P7SEL0 |= BIT5;             // P1.6 as Timer0_B Out1
    P7SEL0 |= BIT6;             // P1.6 as Timer0_B Out1
    P7SEL0 |= BIT7;             // P1.6 as Timer0_B Out1

    /*** Timer A1 Initialization***/
    TA1CCR0 |= period;         // Set Capture Compare 0 for timer b (PWM Period)
    TA1CCTL0 |= OUTMOD_7;
    TA1CTL  |= TIMER_A_CTL_TASSEL_2 + MC_1;    // Set TB0 Control to SMCLK, Up mode

    /*** Timer A1.1 Set-up***/
    TA1CCR1 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA1CCTL1 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set

    /*** Timer A1.2 Set-up***/
    TA1CCR2 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA1CCTL2 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set

    /*** Timer A1.3 Set-up***/
    TA1CCR3 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA1CCTL3 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set

    /*** Timer A1.4 Set-up***/
    TA1CCR4 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA1CCTL4 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set
}

void editMotorPWM(int motor, int duty){
    switch (motor){
    case 0:
        TA1CCR1 = duty;
        break;

    case 1:
        TA1CCR2 = duty;
        break;

    case 2:
        TA1CCR3 = duty;
        break;

    case 3:
        TA1CCR4 = duty;
        break;

    default:
        break;
    }
}

void editMainPWM(int duty){
    if (duty > 100)
        duty = 100;
    if (duty < 0)
        duty = 0;
    duty = duty * MOTOR_PWM_PERIOD / 100;
    TA1CCR1 = duty;
    TA1CCR2 = duty;
    TA1CCR3 = duty;
    TA1CCR4 = duty;
}

void armMotors()
{
    int c = 0;

    while (c < 50){
        int c1 = 0;
        while (c1 < 10000) c1++;
        TA1CCR1 = c;
        TA1CCR2 = c;
        TA1CCR3 = c;
        TA1CCR4 = c;
        c++;
    }
    int c1 = 0;
            while (c1 < 10000000) c1++;
    while (c > 0){
        int c1 = 0;
        while (c1 < 10000) c1++;
        TA1CCR1 = c;
        TA1CCR2 = c;
        TA1CCR3 = c;
        TA1CCR4 = c;
        c --;
    }

//    editMainPWM(20);
//    volatile int32_t c = 7500000;
//    while (c != 0)
//        c--;
//    editMainPWM(70);
//    c = 7500000;
//    while (c != 0)
//        c--;
//    editMainPWM(10);
//    c = 480000;
//    while (c != 0)
//        c--;
//    editMainPWM(20);
}

void disarmMotors()
{
    editMainPWM(20);
    volatile int32_t c = 480000;
    while (c != 0)
        c--;
    editMainPWM(10);
}
