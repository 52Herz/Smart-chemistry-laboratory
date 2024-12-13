#ifndef __USART_2_H
#define __USART_2_H
#endif

#include "stm32f10x.h"                  // Device header
#include <stdio.h> 
#include "stdarg.h"		 
#include "string.h"    


#define USART2_PIN_TX GPIO_Pin_2
#define USART2_PIN_RX GPIO_Pin_3
#define USART2_GPIO GPIOA
#define USART2_AF GPIO_AF_USART2



#define USART2_TXBUFF_SIZE 1024
#define USART2_RXBUFF_SIZE 3072
	
extern char Usart2_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart2_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量

void Usart2_Init(unsigned int bound);   
void Usart2_printf(char* fmt, ...) ;
void Usart2_SendString(char *String);
void WIFI_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void Usart2_SendByte(uint8_t Byte);
void Usart2_SendArray(uint8_t *Array, uint16_t Length);
void Usart2_SendNumber(uint32_t Number, uint8_t Length);