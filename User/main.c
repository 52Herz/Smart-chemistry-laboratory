#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MQ_2.h"
#include "Sht_20.h"
#include "I2c.h"
#include "Usart_1.h"
#include "Usart_2.h"
#include "Wifi.h"
#include "Json_transmit.h"
#include "string.h"


//全局变量
u16 value;
u8 buff[30];//参数显示缓存数组
float ppm;

//json结构体
SensorData sensor_data;

//

int main(void)
{
  
	IIC_Init();
    Usart1_Init(115200);
    Usart2_Init(115200);
		
    MQ2_Init();
    WiFi_ResetIO_Init();

    Usart1_printf("Start \r\n");
	Delay_ms(1000);
	while(WiFi_Connect_IoTServer() != 0){}
    Usart1_printf("wifi_OK");
    while (1)
    {
			value = MQ2_GetData();  
		
			Usart1_printf("烟雾浓度: %d\r\n",value);
			ppm = MQ2_GetData_PPM();
			Usart1_printf("%.2fppm\r\n",ppm);
			SHT20_GetValue();
			Usart1_printf("湿度：%0.1f%%\r\n",sht20_info.humidity);
			Usart1_printf("温度：%0.2f℃\r\n",sht20_info.tempreture);
			
			//json数据传输
			sensor_data.temperature = sht20_info.tempreture;
			sensor_data.humidity = sht20_info.humidity;
			sensor_data.MQ2_ppm = ppm;
			Sensor_Json_Data(sensor_data);
		
		//OLED_ShowString(48,16,buff,16,1);
			
		
        Delay_ms(3000); //3秒发送一轮
    }
    
}



