/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-10-23 12:26:21
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-11-25 08:26:39
 * @FilePath     : \智慧化工实验室\Hardware\MQ_136.c
 * @Description  : 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */

//系统引用库
#include <math.h>

//外设库
#include "MQ_136.h"

// 初始化MQ136模块的ADC
void MQ136_Init(void) {
    // 启用ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA6为模拟输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MQ_136_PIN_AO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置ADC
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // 开启ADC
    ADC_Cmd(ADC1, ENABLE);

    // 校准ADC
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

// 读取ADC值
uint16_t MQ136_ReadADC(void) {
    // 配置ADC通道
    ADC_RegularChannelConfig(ADC1, MQ136_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);

    // 启动ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    // 等待转换完成
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    // 返回转换结果
    return ADC_GetConversionValue(ADC1);
}

// 根据ADC值计算PPM
    float MQ136_CalculatePPM(uint16_t adc_value) {
    float vrl = (float)adc_value / 4095 * VREF;  // 转换为电压值
    float rs = (VREF / vrl - 1) * RL;            // 计算传感器电阻Rs
    float ratio = rs / RO;                       // Rs/Ro的比值

    // 使用传感器的校准曲线，假设log(PPM) = A * log(Rs/Ro) + B
    // A 和 B 需要根据数据手册的校准曲线进行调整
    float A = -0.45;  // 假设值，根据手册校准曲线调整
    float B = 1.25;   // 假设值，根据手册校准曲线调整
    float ppm = pow(10, (log10(ratio) - B) / A); // 计算PPM值

    return ppm;
}
