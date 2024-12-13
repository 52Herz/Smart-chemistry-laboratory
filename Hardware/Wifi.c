
/*-------------------------------------------------*/
/*                                                 */
/*          	 WIFI（ESP8266）源文件             */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"
#include "Delay.h"
#include "Wifi.h"
#include "Usart_1.h"
#include "Usart_2.h"

char wifi_mode = 0; // 联网模式 0：SSID和密码写在程序里   1：Smartconfig方式用APP发送

/*-------------------------------------------------*/
/*函数名：初始化WiFi的复位IO                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

/**
 * @description:
 * @return {无}
 * @instruction:
 */
void WiFi_ResetIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = WIFI_RESET_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_RESET_GPIO, &GPIO_InitStructure);
	RESET_IO(1);
}

/*-------------------------------------------------*/
/*函数名：WiFi发送设置指令                          */
/*参  数：cmd：指令                                */
/*参  数：timeout：超时时间（100ms的倍数）          */
/*返回值：0：正确   其他：错误                      */
/*-------------------------------------------------*/
/**
 * @description:
 * @param    {char} *cmd*
 * @param    {int} timeout*
 * @return   {无}
 * @instruction:
 */
_Bool WiFi_SendCmd(char *cmd, int timeout)
{
	WIFI_RXCOUNTER = 0;
	memset(WIFI_RX_BUF, 0, WIFI_RXBUF_SIZE); // WiFi接收数据量变量清零
	WIFI_Prinf("%s\r\n", cmd);
	while (timeout--)
	{
		Delay_ms(100);
		if (strstr(WIFI_RX_BUF, "OK"))
			break;
		if (strstr(WIFI_RX_BUF, "ready")) // 为AT+RST复位指令准备
			break;
		DEBUG_Printf("%d ", timeout);

		//		DEBUG_Printf("%s",WIFI_RX_BUF);

		for (int i = 0; WIFI_RX_BUF[i] != '\0'; i++) // 遍历字符数组（字符串），遇到字符串结束标志位后停止
		{
			DEBUG_Printf("%s", WIFI_RX_BUF[i]);
		}
	}

	DEBUG_Printf("\r\n");
	if (timeout <= 0)
		return 1;
	else
		return 0;
}

/*-------------------------------------------------*/
/*函数名：WiFi复位                                 */
/*参  数：timeout：超时时间（100ms的倍数）          */
/*返回值：0：正确   其他：错误                      */
/*-------------------------------------------------*/
_Bool WiFi_Reset(int timeout)
{
	WIFI_RXCOUNTER = 0;
	memset(WIFI_RX_BUF, 0, WIFI_RXBUF_SIZE); // WiFi接收数据量变量清零

	RESET_IO(0);
	Delay_ms(500);
	RESET_IO(1);
	Delay_ms(500);

	while (timeout--)
	{
		Delay_ms(100);
		if (strstr(WIFI_RX_BUF, "ready"))
			break;
		DEBUG_Printf("TimeOut：%d ", timeout);

		// 临时调试
		//			for (int i = 0; WIFI_RX_BUF[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
		//			{
		//					DEBUG_Printf("%s",WIFI_RX_BUF[i]);
		//			}
	}

	DEBUG_Printf("\r\n");
	if (timeout <= 0)
		return 1;
	else
		return 0;
}

////==========================================================
////	函数名称：	ESP8266_WaitRecive
////
////	函数功能：	等待接收完成
////
////	入口参数：	无
////
////	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
////
////	说明：		循环调用检测是否接收完成
////==========================================================
//_Bool ESP8266_WaitRecive(void)
//{

//	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
//		return REV_WAIT;
//
//	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
//	{
//		esp8266_cnt = 0;							//清0接收计数
//
//		return REV_OK;								//返回接收完成标志
//	}
//
//	esp8266_cntPre = esp8266_cnt;					//置为相同
//
//	return REV_WAIT;								//返回接收未完成标志

//}

////==========================================================
////	函数名称：	ESP8266_SendCmd
////
////	函数功能：	发送命令
////
////	入口参数：	cmd：命令
////				res：需要检查的返回指令
////
////	返回参数：	0-成功	1-失败
////
////	说明：
////==========================================================
//_Bool ESP8266_SendCmd(char *cmd, char *res)
//{
//
//	unsigned char timeOut = 200;

//	WIFI_Prinf((unsigned char *)cmd, strlen((const char *)cmd));
//
//	while(timeOut--)
//	{
//		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
//		{
//			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
//			{
//				ESP8266_Clear();									//清空缓存
//
//				return 0;
//			}
//		}
//
//		DelayXms(10);
//	}
//
//	return 1;

//}

/*-------------------------------------------------*/
/*函数名：WiFi加入路由器指令                       */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_JoinAP(int timeout)
{
	WIFI_RXCOUNTER = 0;
	memset(WIFI_RX_BUF, 0, WIFI_RXBUF_SIZE);
	WIFI_Prinf("AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD);

	while (timeout--)
	{
		Delay_ms(1000);
		if (strstr(WIFI_RX_BUF, "OK"))
			break;
		DEBUG_Printf("%d ", timeout);
	}
	DEBUG_Printf("\r\n%s\r\n", WIFI_RX_BUF);
	DEBUG_Printf("\r\n");
	if (timeout <= 0)
		return 1;
	else
		return 0;
}

/*-------------------------------------------------*/
/*函数名：连接TCP服务器，并进入透传模式            */
/*参  数：timeout： 超时时间（100ms的倍数）        */
/*返回值：2：正确  其他：错误                      */
/*-------------------------------------------------*/
char WiFi_Connect_Server(int timeout)
{
	WIFI_RXCOUNTER = 0;
	memset(WIFI_RX_BUF, 0, WIFI_RXBUF_SIZE);
	WIFI_Prinf("AT+CIPSTART=\"%s\",\"%s\",%d\r\n", CONNECT_MODE, ServerIP, ServerPort); // 发送连接服务器指令

	while (timeout--)
	{
		Delay_ms(1000);
		if (strstr(WIFI_RX_BUF, "CONNECT"))			  // 如果接受到CONNECT表示连接成功
			break;									  // 跳出while循环
		if (strstr(WIFI_RX_BUF, "CLOSED"))			  // 如果接受到CLOSED表示服务器未开启
			return 1;								  // 服务器未开启返回1
		if (strstr(WIFI_RX_BUF, "ALREADY CONNECTED")) // 如果接受到ALREADY CONNECTED已经建立连接
			return 2;								  // 已经建立连接返回2
		DEBUG_Printf("%d ", timeout);
	}
	DEBUG_Printf("\r\n"); // 串口输出信息
	if (timeout <= 0)
		return 3; // 超时错误，返回3
	else
	{
		DEBUG_Printf("连接服务器成功，准备进入透传\r\n"); // 串口显示信息
		WIFI_RXCOUNTER = 0;								  // WiFi接收数据量变量清零
		memset(WIFI_RX_BUF, 0, WIFI_RXBUF_SIZE);		  // 清空WiFi接收缓冲区
		WIFI_Prinf("AT+CIPSEND\r\n");					  // 发送进入透传指令
		while (timeout--)								  // 等待超时与否
		{
			Delay_ms(100);								// 延时100ms
			if (strstr(WIFI_RX_BUF, "\r\nOK\r\n\r\n>")) // 如果成立表示进入透传成功
				break;									// 跳出while循环
			DEBUG_Printf("%d ", timeout);				// 串口输出现在的超时时间
		}
		if (timeout <= 0)
			return 4; // 透传超时错误，返回4
	}
	return 0;
}

/*-------------------------------------------------*/
/*函数名：WiFi_Smartconfig                         */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/

/*-------------------------------------------------*/
/*函数名：等待加入路由器                           */
/*参  数：timeout：超时时间（1s的倍数）            */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/
char WiFi_WaitAP(int timeout)
{
	while (timeout--)
	{											// 等待超时时间到0
		Delay_ms(1000);							// 延时1s
		if (strstr(WIFI_RX_BUF, "WIFI GOT IP")) // 如果接收到WIFI GOT IP表示成功
			break;								// 主动跳出while循环
		DEBUG_Printf("%d ", timeout);			// 串口输出现在的超时时间
	}
	DEBUG_Printf("\r\n"); // 串口输出信息
	if (timeout <= 0)
		return 1; // 如果timeout<=0，说明超时时间到了，也没能收到WIFI GOT IP，返回1
	return 0;	  // 正确，返回0
}

/*-------------------------------------------------*/
/*函数名：WiFi连接服务器                           */
/*参  数：无                                       */
/*返回值：0：正确   其他：错误                     */
/*-------------------------------------------------*/

/**
 * @brief     :
 * @param      [*]
 * @return     [*]
 * @instruction:
 */
char WiFi_Connect_IoTServer(void)
{
	/******硬件复位模块暂时有问题，可以成功复位，无法从返回数据中读到ready*******/
	DEBUG_Printf("准备检查模块\r\n"); // 串口提示数据
	if (WiFi_SendCmd("AT", 50))		  // 检查，100ms超时单位，总计5s超时时间
	{
		DEBUG_Printf("检查状态失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
		return 1;									// 返回1
	}
	else
		DEBUG_Printf("检查成功\r\n"); // 串口提示数据

	//	DEBUG_Printf("准备复位模块\r\n");					 //串口提示数据
	//	if(WiFi_SendCmd("AT+RST",50))						//复位，100ms超时单位，总计5s超时时间
	//	{
	//		DEBUG_Printf("复位失败，准备重启\r\n");	      //返回非0值，进入if，串口提示数据
	//		return 2;                                   //返回2
	//	}else DEBUG_Printf("复位成功\r\n");                 //串口提示数据
	//
	/******复位模块暂时有问题，暂时更改*******/
	// Usart2_printf("AT+RST\r\n");
	// Delay_ms(1000);
	// DEBUG_Printf("复位成功\r\n");

	DEBUG_Printf("准备设置STA模式\r\n"); // 串口提示数据
	if (WiFi_SendCmd("AT+CWMODE=1", 50)) // 设置STA模式，100ms超时单位，总计5s超时时间
	{
		DEBUG_Printf("设置STA模式失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
		return 3;									   // 返回3
	}
	else
		DEBUG_Printf("设置STA模式成功\r\n"); // 串口提示数据

	if                       (wifi_mode == 0) // 如果联网模式=0：SSID和密码写在程序里
	{
		DEBUG_Printf("准备取消自动连接\r\n");	 // 串口提示数据
		if (WiFi_SendCmd("AT+CWAUTOCONN=0", 50)) // 取消自动连接，100ms超时单位，总计5s超时时间
		{
			DEBUG_Printf("取消自动连接失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
			return 4;										// 返回4
		}
		else
			DEBUG_Printf("取消自动连接成功\r\n"); // 串口提示数据

		DEBUG_Printf("准备连接路由器\r\n"); // 串口提示数据
		if (WiFi_JoinAP(30))				// 连接路由器,1s超时单位，总计30s超时时间
		{
			DEBUG_Printf("连接路由器失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
			return 5;									  // 返回45
		}
		else
			DEBUG_Printf("连接路由器成功\r\n"); // 串口提示数据
	}

	DEBUG_Printf("准备设置透传\r\n");	  // 串口提示数据
	if (WiFi_SendCmd("AT+CIPMODE=1", 50)) // 设置透传，100ms超时单位，总计5s超时时间
	{
		DEBUG_Printf("设置透传失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
		return 6;									// 返回6
	}
	else
		DEBUG_Printf("设置透传成功\r\n"); // 串口提示数据

	DEBUG_Printf("准备关闭多路连接\r\n"); // 串口提示数据
	if (WiFi_SendCmd("AT+CIPMUX=0", 50))  // 关闭多路连接，100ms超时单位，总计5s超时时间
	{
		DEBUG_Printf("关闭多路连接失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
		return 7;										// 返回7P
	}
	else
		DEBUG_Printf("关闭多路连接成功\r\n"); // 串口提示数据

	DEBUG_Printf("准备连接服务器\r\n"); // 串口提示数据
	if (WiFi_Connect_Server(100))		// 连接服务器，100ms超时单位，总计10s超时时间
	{
		DEBUG_Printf("连接服务器失败，准备重启\r\n"); // 返回非0值，进入if，串口提示数据
		return 10;									  // 返回10
	}
	else
		DEBUG_Printf("连接服务器成功\r\n"); // 串口提示数据
	return 0;								// 正确返回0
}

//==========================================================
//	函数名称：	USART2_IRQHandler
//
//	函数功能：	串口2收发中断
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：
//==========================================================
void USART2_IRQHandler(void)
{

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断
	{
		if (WIFI_RXCOUNTER >= sizeof(WIFI_RX_BUF))
			WIFI_RXCOUNTER = 0; // 防止串口被刷爆
		WIFI_RX_BUF[WIFI_RXCOUNTER++] = USART_ReceiveData(USART2);

		USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除标志位
	}
}