/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-10-16 21:29:46
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-12-12 13:03:15
 * @FilePath     : \智慧化工实验室\User\main.c
 * @Description  : 
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "MQ_2.h"
#include "Sht_20.h"
#include "I2c.h"
#include "Usart_1.h"
#include "Usart_2.h"
#include "Wifi.h"
#include "Json_transmit.h"
#include "string.h"
#include "LED.h"
#include "OLED.h"


// 全局变量
u16 value;
u8 buff[30]; // 参数显示缓存数组
float ppm;

// json结构体
SensorData sensor_data;

//
// int main()
// {
// 	//设备初始化
// 	OLED_Init();
// 	Usart1_Init(115200);


// 	/*设备初始化end*/

// 	OLED_ShowNum(1,1,123,5);
// 	OLED_ShowNum(2,1,23,5);


// 	while (1)
// 	{
// 		SHT20_GetValue();
		
// 		float value;
// 		value = sht20_info.humidity;
// 		OLED_ShowNum(3,1,value,4);	

// 		Usart1_printf("%f",sht20_info.humidity);
// 		Delay_ms(500);

// 	}
	
	
// }


int main(void)
{
	LED_Init();
	IIC_Init();
	Usart1_Init(115200);
	Usart2_Init(115200);

	MQ2_Init();
	WiFi_ResetIO_Init();

	Usart1_printf("Start \r\n");

	while (WiFi_Connect_IoTServer() != 0)
	{
		Usart1_printf("wifi_Error\r\n");
		Delay_s(1);
	}
	Usart1_printf("wifi_OK\r\n");
	LED1_ON();
	Delay_s(1);

	while (1)
	{
		value = MQ2_GetData();

		Usart1_printf("烟雾浓度: %d\r\n", value);
		ppm = MQ2_GetData_PPM();
		Usart1_printf("%.2fppm\r\n",ppm);
		SHT20_GetValue();
		Usart1_printf("湿度：%0.1f%%\r\n",sht20_info.humidity);
		Usart1_printf("温度：%0.2f℃\r\n",sht20_info.tempreture);

		// json数据传输
		sensor_data.temperature = sht20_info.tempreture;
		sensor_data.humidity = sht20_info.humidity;
		sensor_data.MQ2_ppm = ppm;
		Sensor_Json_Data(sensor_data);

		Delay_ms(2000); // 3秒发送一轮
	}
}
