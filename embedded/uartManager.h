/*
 * uart.h
 *
 *  Created on: Sep 30, 2020
 *      Author: Alex Magyari
 */

#ifndef UARTMANAGER_H_
#define UARTMANAGER_H_


// Initialize UART on ports 1.2 and 1.3
void UART_init(void);

// Send string to PC over UART. Strings must be null terminated
void UART2PCString(char * data);

// Send char to PC over UART
void UART2PCChar(uint8_t character);

// Send newline and carriage return to PC
void UART2PCNewLine(void);

// Send float to PC
void UART2PCFloat(float data);

#endif /* UARTMANAGER_H_ */
