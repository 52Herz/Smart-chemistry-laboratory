#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

//宏定义
#define USART1_PIN_TX GPIO_Pin_9
#define USART1_PIN_RX GPIO_Pin_10

extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

#endif
