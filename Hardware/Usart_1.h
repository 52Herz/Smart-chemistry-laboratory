#ifndef __USART_1_H
#define __USART_1_H
#endif

#include "stm32f10x.h"                  // Device header
#include <stdio.h> 
#include "stdarg.h"		 
#include "string.h"    


#define USART1_PIN_TX GPIO_Pin_9
#define USART1_PIN_RX GPIO_Pin_10
#define USART1_GPIO GPIOA
#define USART1_AF GPIO_AF_USART1



#define USART1_TXBUFF_SIZE 1024
#define USART1_RXBUFF_SIZE 1024
	
extern char Usart1_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart1_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量

void Usart1_Init(unsigned int bound);   
void Usart1_printf(char* fmt, ...) ;
void Usart1_SendByte(uint8_t Byte);
void Usart1_SendArray(uint8_t *Array, uint16_t Length);
void Usart1_SendNumber(uint32_t Number, uint8_t Length);