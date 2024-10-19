#include "stm32f10x.h"                  // Device header
#include "Usart_2.h"

char Usart2_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart2_RxCounter = 0;      //定义一个变量，记录串口2总共接收了多少字节的数据
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //定义一个数组，用于保存串口2接收到的数据   

void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //定义一个设置GPIO功能的变量
	USART_InitTypeDef USART_InitStructure;   //定义一个设置串口功能的变量
	NVIC_InitTypeDef NVIC_InitStructure;     //如果使能接收功能，定义一个设置中断的变量

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3	
      
	
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能USART2_GPIO时钟
	USART_DeInit(USART2);                                  //串口2寄存器重新设置为默认值
    GPIO_InitStructure.GPIO_Pin = USART2_PIN_TX;              //准备设置PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO速率50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //复用推挽输出，用于串口2的发送
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);                 //设置PA3
      
    GPIO_InitStructure.GPIO_Pin = USART2_PIN_RX;              //准备设置PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入，用于串口2的接收
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);                 //设置PA10
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8个数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //收发模式
      
  USART_Init(USART2, &USART_InitStructure);                                      //设置串口2	

	USART_ClearFlag(USART2, USART_FLAG_RXNE);	              //清除接收标志位
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);            //开启接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //设置串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //中断通道使能
	NVIC_Init(&NVIC_InitStructure);	                          //设置串口2中断 

	USART_Cmd(USART2, ENABLE);                                //使能串口2
}

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Usart2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

void Usart2_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Usart2_SendByte(Array[i]);		//依次调用Usart1_SendByte发送每个字节数据
	}
}

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Usart_2_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}


/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Usart2_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Usart2_SendByte(Number / Usart_2_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Usart_1_SendByte发送每位数字
	}
}

/*-------------------------------------------------*/
/*函数名：串口2发送缓冲区中的数据                    */
/*参  数：data：数据                                */
/*返回值：无                                        */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	int	i;	
	while((USART1->SR&0X40) == 0);
	for(i = 1; i <= (data[0] * 256 + data[1]); i++)
	{			
		USART1->DR = data[i+1];
		while((USART1->SR&0X40) == 0);	
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Usart2_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Usart2_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void WIFI_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}



/*-------------------------------------------------*/
/*函数名：串口2 printf函数                          */
/*参  数：char* fmt,...  格式化输出字符串和参数      */
/*返回值：无                                        */
/*-------------------------------------------------*/

__align(8) char Usart2_TxBuff[USART2_TXBUFF_SIZE];  

void Usart2_printf(char* fmt, ...) 
{  
	unsigned int i, length;
	
	va_list ap;
	va_start(ap, fmt);
	vsprintf(Usart2_TxBuff, fmt, ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart2_TxBuff);		
	while((USART2->SR&0X40) == 0);
	for(i = 0; i < length; i++)
	{			
		USART2->DR = Usart2_TxBuff[i];
		while((USART2->SR&0X40) == 0);	
	}	
}
