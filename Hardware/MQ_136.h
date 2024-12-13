/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-10-23 12:26:04
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-12-03 10:45:04
 * @FilePath     : \智慧化工实验室\Hardware\MQ_136.h
 * @Description  : 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __MQ_136_H
#define __MQ_136_H

//系统引用库
#include <math.h>

//私有库
#include "stm32f10x.h"

//引脚功能宏定义
#define MQ_136_PIN_AO GPIO_Pin_6  //设置PA6为模拟输入引脚


// 全局宏定义
#define MQ136_ADC_CHANNEL     ADC_Channel_6   // 使用PA6引脚
#define RL                    10000.0         // 负载电阻值，单位为欧姆
#define RO                    15000.0         // 空气中的传感器参考电阻，单位为欧姆
#define VREF                  5            // 参考电压（假设STM32参考电压为3.3V） //TODO:计算公式不正确需要重写

// 函数声明
void MQ136_Init(void);
uint16_t MQ136_ReadADC(void);
float MQ136_CalculatePPM(uint16_t adc_value);

#endif
