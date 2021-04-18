#include "stm32f4xx.h"
#include "cJSON.h"
#include "string.h"

static GPIO_InitTypeDef  GPIO_InitStructure;
static USART_InitTypeDef USART_InitStructure;
static NVIC_InitTypeDef NVIC_InitStructure;


void delay(void){
	uint32_t i = 0x2000000;
	
	while(i--);
}

void usart1_init(uint32_t baud)
{
	//打开PA硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//打开串口1硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//配置PA9和PA10为复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //第9,10引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//多功能模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推完输出,提高输出电流能力
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不需要上下拉电阻
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//将PA9和PA10引脚连接到串口1的硬件
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// 配置串口1相关参数,波特率,无校验位,8位数据位,1个停止位..
   USART_InitStructure.USART_BaudRate = baud; //波特率
   USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8个数据位
   USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1个停止位
   USART_InitStructure.USART_Parity = USART_Parity_No;		//无奇偶校验
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//允许收发数据
   USART_Init(USART1, &USART_InitStructure);


	// 配置串口1的中断触发方法, 接收一个字节出发终端
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	// 配置串口1的终端优先级
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   
   //使能串口1工作
   USART_Cmd(USART1, ENABLE);

}

int main(){
	//使能端口G的供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//初始化GPIO引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推完输出,提高输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不需要上下拉电阻
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//初始化接收数据
	usart1_init(115200);
	
	//发送数据
	USART_SendData(USART1,'G');
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,'E');
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,'C');
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	
	while(1){
		//PA6引脚输出高电平
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		delay();
		//PA6输出低电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		delay();
	}
}
