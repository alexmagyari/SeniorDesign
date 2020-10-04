#include <ti/devices/msp432p4xx/inc/msp.h>

/*
 * motor.c
 *
 *  Created on: Sep 30, 2020
 *      Author: Alexander Magyari
 */


void initPWM(int period, int startDuty){


    /*** GPIO Setup***/
    P2DIR |= BIT4;              // P1.6 as output
    P2DIR |= BIT5;              // P1.6 as output
    P2DIR |= BIT6;              // P1.6 as output
    P2DIR |= BIT7;              // P1.6 as output

    P2SEL0 |= BIT4;             // P1.6 as Timer0_B Out1
    P2SEL0 |= BIT5;             // P1.6 as Timer0_B Out1
    P2SEL0 |= BIT6;             // P1.6 as Timer0_B Out1
    P2SEL0 |= BIT7;             // P1.6 as Timer0_B Out1

    /*** Timer A0 Initialization***/
    TA0CCR0 |= period;         // Set Capture Compare 0 for timer b (PWM Period)
    TA0CCTL0 |= OUTMOD_7;
    TA0CTL  |= TIMER_A_CTL_TASSEL_2 + MC_1;    // Set TB0 Control to SMCLK, Up mode

    /*** Timer A0.1 Set-up***/
    TA0CCR1 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA0CCTL1 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set

    /*** Timer A0.2 Set-up***/
    TA0CCR2 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA0CCTL2 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set

    /*** Timer A0.3 Set-up***/
    TA0CCR3 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA0CCTL3 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set

    /*** Timer A0.4 Set-up***/
    TA0CCR4 |= startDuty;              // Set Capture Compare 1 for timer b (Max counts the signal is high)
    TA0CCTL4 |= OUTMOD_7;       // Set Capture/Compare Control 0 to reset/set
}

void editMotorPWM(int motor, int duty){
    switch (motor){
    case 0:
        TA0CCR1 = duty;
        break;

    case 1:
        TA0CCR2 = duty;
        break;

    case 2:
        TA0CCR3 = duty;
        break;

    case 3:
        TA0CCR4 = duty;
        break;

    default:
        break;
    }
}

void editMainPWM(int duty){
    TA0CCR1 = duty;
    TA0CCR2 = duty;
    TA0CCR3 = duty;
    TA0CCR4 = duty;
}

