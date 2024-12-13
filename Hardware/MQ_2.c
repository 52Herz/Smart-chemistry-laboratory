/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-10-17 15:42:48
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-11-25 08:31:41
 * @FilePath     : \智慧化工实验室\Hardware\MQ_2.c
 * @Description  : 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "MQ_2.h"

void MQ2_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 模式选择：模拟AO: 1 数字DO：0
    #if MODE
        RCC_APB2PeriphClockCmd(MQ2_AO_GPIO_CLK, ENABLE);  // 打开 ADC IO端口时钟
        GPIO_InitStructure.GPIO_Pin = MQ2_AO_GPIO_PIN;     // 配置 ADC IO 引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       // 设置为模拟输入
        GPIO_Init(MQ2_AO_GPIO_PORT, &GPIO_InitStructure);   // 初始化 ADC IO
        ADCx_Init();                                         // 初始化 ADC
    #else
        RCC_APB2PeriphClockCmd(MQ2_DO_GPIO_CLK, ENABLE);    // 打开连接传感器DO的单片机引脚端口时钟
        GPIO_InitStructure.GPIO_Pin = MQ2_DO_GPIO_PIN;      // 配置连接传感器DO的单片机引脚模式
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       // 设置为上拉输入
        GPIO_Init(MQ2_DO_GPIO_PORT, &GPIO_InitStructure);   // 初始化 DO 引脚
    #endif
}

#if MODE
uint16_t MQ2_ADC_Read(void) {
    // 设置指定ADC的规则组通道，采样时间
    return ADC_GetValue(ADC_CHANNEL, ADC_SampleTime_55Cycles5);
}
#endif

uint16_t MQ2_GetData(void) {
    #if MODE
        uint32_t tempData = 0;

        // 循环读取ADC值
        for (uint8_t i = 0; i < MQ2_READ_TIMES; i++) {
            tempData += MQ2_ADC_Read();
            Delay_ms(5);
        }

        // 返回平均值
        return tempData / MQ2_READ_TIMES;
    #else
        return !GPIO_ReadInputDataBit(MQ2_DO_GPIO_PORT, MQ2_DO_GPIO_PIN);
    #endif
}

float MQ2_GetData_PPM(void) {
    #if MODE
        uint32_t tempData = 0;

        // 循环读取ADC值
        for (uint8_t i = 0; i < MQ2_READ_TIMES; i++) {
            tempData += MQ2_ADC_Read();
            Delay_ms(5);
        }

        // 计算平均电压
        float averageVoltage = (tempData / MQ2_READ_TIMES) * 5.0f / 4096.0f;
        float RS = (5.0f - averageVoltage) / (averageVoltage * 0.5f);
        const float R0 = 6.64f;  // 参考电阻值

        // 计算PPM
        return pow((11.5428f * R0) / RS, 0.6549f);
    #endif
}
