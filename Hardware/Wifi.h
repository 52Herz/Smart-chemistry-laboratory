/*
 * @Author       : 52Herz 1065021993@qq.com
 * @Date         : 2024-10-20 00:41:32
 * @LastEditors  : 52Herz 1065021993@qq.com
 * @LastEditTime : 2024-12-12 13:04:37
 * @FilePath     : \智慧化工实验室\Hardware\Wifi.h
 * @Description  :  
 * 
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __WIFI_H
#define __WIFI_H

#define WIFI_RESET_PIN    GPIO_Pin_4
#define WIFI_RESET_GPIO   GPIOA
#define RESET_IO(signal)  GPIO_WriteBit(GPIOA,GPIO_Pin_4,(BitAction)signal)
#define WIFI_Prinf		  Usart2_printf				    //串口2连接ESP8266输出
#define DEBUG_Printf	  Usart1_printf			        //串口1作为调试串口输出
#define WIFI_RXCOUNTER    Usart2_RxCounter			    //用来对于串口接收数据计数
#define WIFI_RX_BUF       Usart2_RxBuff
#define WIFI_RXBUF_SIZE   USART2_RXBUFF_SIZE

#define REV_WAIT          0                             //接收超时未完成
#define REV_OK            1                             //接收完成


#define WIFI_SSID         "CC5G-OXx"					//WIFI名称 不能用中文
#define WIFI_PASSWORD     "12345678"				    //WIFI密码

#define CONNECT_MODE      "TCP"							//连接方式
#define ServerIP          "192.168.66.114"             //改成服务器地址
#define ServerPort        5000							//服务器端口号

//功能函数
char WiFi_Connect_IoTServer(void);
void WiFi_ResetIO_Init(void);
_Bool WiFi_Reset(int timeout);
char WiFi_JoinAP(int timeout);
_Bool WiFi_SendCmd(char *cmd,int timeout);
#endif