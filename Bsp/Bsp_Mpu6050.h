/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-11-26 12:48:39
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-12-03 14:25:37
 * @FilePath     : \智慧化工实验室\Bsp\Bsp_Mpu6050.h
 * @Description  : 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */


#ifndef __BSP_MPU6050_H__
#define __BSP_MPU6050_H__

//系统引用库



//文件库
#include "stm32f10x.h"

//I2C硬件抽象层结构体
typedef struct
{
    int8_t (*pfinit)(void); //初始化
    int8_t (*pfdeInit)(void);
    int8_t (*pfwriteReg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len); //写入
    int8_t (*pfreadReg)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len); //读取

} Icc_driver_interface_t;
 
//中断抽象层
typedef struct
{
    int8_t (*pfinit)(void); //初始化
    int8_t (*pfdeInit)(void);
    int8_t (*pfintcallback)(void *(pfcallback_after_data_available)(void));
} interrupt_inierface_t;


//BSP Bsp_Mpu6050抽象层结构体
typedef struct 
{
    int8_t (*pfinit)(void); //初始化
    int8_t (*pfdeInit)(void);
    int8_t (*pfstart_working)();
    int8_t (*pfstop_working)();
    int8_t (*pfcheck_status)(void);
    int8_t (*pfset_gyro_fsr)(void);
    int8_t (*pfset_accel_fsr)(void);
    int8_t (*pfset_lpf)(void);
    

    
}bsp_mpu6050_t;


#endif /*__BSP_MPU6050_H__*/

