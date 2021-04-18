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
	//��PAӲ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//�򿪴���1Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//����PA9��PA10Ϊ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //��9,10����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//�๦��ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������,��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������Ӧ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����Ҫ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//��PA9��PA10�������ӵ�����1��Ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// ���ô���1��ز���,������,��У��λ,8λ����λ,1��ֹͣλ..
   USART_InitStructure.USART_BaudRate = baud; //������
   USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8������λ
   USART_InitStructure.USART_StopBits = USART_StopBits_1;	//1��ֹͣλ
   USART_InitStructure.USART_Parity = USART_Parity_No;		//����żУ��
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�����շ�����
   USART_Init(USART1, &USART_InitStructure);


	// ���ô���1���жϴ�������, ����һ���ֽڳ����ն�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	// ���ô���1���ն����ȼ�
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   
   //ʹ�ܴ���1����
   USART_Cmd(USART1, ENABLE);

}

int main(){
	//ʹ�ܶ˿�G�Ĺ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//��ʼ��GPIO����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������,������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������Ӧ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����Ҫ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//��ʼ����������
	usart1_init(115200);
	
	//��������
	USART_SendData(USART1,'G');
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,'E');
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1,'C');
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	
	while(1){
		//PA6��������ߵ�ƽ
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		delay();
		//PA6����͵�ƽ
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		delay();
	}
}
