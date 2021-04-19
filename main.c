#include "stm32f4xx.h"
#include "cJSON.h"
#include "string.h"
#include <stdio.h>

static GPIO_InitTypeDef  GPIO_InitStructure;
static USART_InitTypeDef USART_InitStructure;
static NVIC_InitTypeDef NVIC_InitStructure;

int ret;

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
   //NVIC_Init(&NVIC_InitStructure);
   
   //ʹ�ܴ���1����
   USART_Cmd(USART1, ENABLE);

}

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}

int cjson_parse(const char *value)
{
	//const char *jstr = "{\"cmd\":1,\"fp_temp\":[4,20,94,1,45,202,181,232,67,171,1,248,41,186,182,229,129,200,255,248,38,162,184,1,3,187,191,248,146,178,182,233,67,185,133,248,186,162,182,113,195,57,246,23,198,18,184,4,130,186,69,248,214,170,181,230,131,200,5,224,10,27,183,0,68,186,71,224,38,115,180,223,130,249,67,248,170,51,181,231,67,248,5,224,166,235,181,109,195,72,120,23,25,172,182,8,7,136,69,231,70,172,183,129,133,55,52,0,113,180,184,24,67,169,191,238,122,204,180,226,4,248,197,231,121,84,183,15,133,135,197,239,197,108,181,234,5,247,201,231,205,12,182,9,134,118,193,215,77,115,182,112,132,73,120,23,126,51,182,233,67,185,71,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,49,68,81,67,20,83,50,68,4,66,161,42,100,129,40,98,67,9,84,21,37,131,52,38,21,23,69,86,50,52,42,145,69,121,74,85,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,98]}";
	//const char *jstr = "{\"list\":[\"name\",\"class\",\"class\",\"class\",\"class\",\"cl\",\"c\"]}";
	cJSON *json;
	
	json = cJSON_Parse(value);
	
	if(json == NULL){
		printf("json fmt error:%s\n.", cJSON_GetErrorPtr());
		return -1;
	} else {
		cJSON *cmd = cJSON_GetObjectItem(json, "cmd");
		cJSON *finger = cJSON_GetObjectItem(json, "fp_temp");
		
		cJSON_Delete(json);
	}
	
	return 1;
}

const char *jstr = "{\"cmd\":1,\"fp_temp\":[4,20,94,1,45,202,181,232,67,171,1,248,41,186,182,229,129,200,255,248,38,162,184,1,3,187,191,248,146,178,182,233,67,185,133,248,186,162,182,113,195,57,246,23,198,18,184,4,130,186,69,248,214,170,181,230,131,200,5,224,10,27,183,0,68,186,71,224,38,115,180,223,130,249,67,248,170,51,181,231,67,248,5,224,166,235,181,109,195,72,120,23,25,172,182,8,7,136,69,231,70,172,183,129,133,55,52,0,113,180,184,24,67,169,191,238,122,204,180,226,4,248,197,231,121,84,183,15,133,135,197,239,197,108,181,234,5,247,201,231,205,12,182,9,134,118,193,215,77,115,182,112,132,73,120,23,126,51,182,233,67,185,71,232,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,49,68,81,67,20,83,50,68,4,66,161,42,100,129,40,98,67,9,84,21,37,131,52,38,21,23,69,86,50,52,42,145,69,121,74,85,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,98]}";
int i;
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
	printf("--- CJSON Test %s---\r\n",__TIME__);
	printf("start\r\n");
	for(i=0;i<100;i++){
		ret = cjson_parse(jstr);
	}
	printf("end\r\n");
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
