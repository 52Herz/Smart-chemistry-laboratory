/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-10-17 15:43:02
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-11-25 08:58:53
 * @FilePath     : \智慧化工实验室\Hardware\MQ_2.h
 * @Description  : 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __MQ2_H
#define __MQ2_H

#include <math.h>

#include "stm32f10x.h"
#include "Adcx.h"
#include "Delay.h"


#define MQ2_READ_TIMES 10  // MQ-2传感器ADC循环读取次数
#define V_REF 5.0          // 定义参考电压为5V

// 模式选择	
// 模拟AO:	1
// 数字DO:	0
#define MODE 1

/***************根据自己需求更改****************/ 
// MQ-2 GPIO宏定义
#if MODE
#define MQ2_AO_GPIO_CLK    RCC_APB2Periph_GPIOA
#define MQ2_AO_GPIO_PORT   GPIOA
#define MQ2_AO_GPIO_PIN    GPIO_Pin_0
#define ADC_CHANNEL        ADC_Channel_0  // ADC 通道宏定义

#else
#define MQ2_DO_GPIO_CLK    RCC_APB2Periph_GPIOA
#define MQ2_DO_GPIO_PORT   GPIOA
#define MQ2_DO_GPIO_PIN    GPIO_Pin_1
#endif //模式设置
/*********************END**********************/ 

void MQ2_Init(void);
uint16_t MQ2_GetData(void);
float MQ2_GetData_PPM(void);

#endif /* __MQ2_H */
