#ifndef __WIFI_H
#define __WIFI_H

#define WIFI_RESET_PIN    GPIO_Pin_4
#define WIFI_RESET_GPIO   GPIOA
#define RESET_IO(signal)  GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)signal)
#define WIFI_Prinf		    Usart2_printf					//串口2连接ESP8266输出
#define DEBUG_Printf	    Usart1_printf					//串口1作为调试串口输出
#define WIFI_RXCOUNTER    Usart2_RxCounter			//用来对于串口接收数据计数
#define WIFI_RX_BUF       Usart2_RxBuff
#define WIFI_RXBUF_SIZE   USART2_RXBUFF_SIZE

#define WIFI_SSID         "ab"					//WIFI名称 不能用中文
#define WIFI_PASSWORD     "12345678"				//WIFI密码

#define CONNECT_MODE      "TCP"							//连接方式
#define ServerIP          "192.168.151.104"   //改成服务器地址
#define ServerPort        8080							//服务器端口号

//功能函数
char WiFi_Connect_IoTServer(void);
void WiFi_ResetIO_Init(void);
_Bool WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
_Bool WiFi_SendCmd(char *cmd,int timeout);
#endif