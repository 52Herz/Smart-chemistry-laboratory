#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MQ_2.h"
#include "Sht_20.h"
#include "I2c.h"
#include "Usart_1.h"
#include "Usart_2.h"
#include "ESP8266.h"
#include "string.h"


//全局变量
u16 value;
u8 buff[30];//参数显示缓存数组
float ppm;
//

int main(void)
{
  
		IIC_Init();
    Usart1_Init(115200);
    Usart2_Init(115200);

    MQ2_Init();
    ESP8266_Init();

    Usart1_printf("Start \r\n");
		Delay_ms(1000);

    while (1)
    {
        
		value = MQ2_GetData();  
		
		Usart1_printf("烟雾浓度: %d\r\n",value);
		//OLED_ShowNum(80,0,value,4,16,1);
		
		ppm = MQ2_GetData_PPM();
		Usart1_printf("%.2fppm\r\n",ppm);
		
    SHT20_GetValue();
    Usart1_printf("湿度：%0.1f%%\r\n",sht20_info.humidity);
    Usart1_printf("温度：%0.2f℃\r\n",sht20_info.tempreture);

		
		//OLED_ShowString(48,16,buff,16,1);
			
		
        Delay_ms(2000);
    }
    
}



