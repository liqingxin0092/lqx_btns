#include "uart.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "stm32f10x.h"

/*����1*/
volatile uint8_t uart1_receive;
volatile uint16_t uart1_current_length;    
volatile uint8_t uart1_buffer[UART1_BUFFER_LENGTH];

/*����2*/
volatile uint8_t  uart2_receive;
volatile uint16_t uart2_current_length;   
volatile uint8_t  uart2_buffer[UART2_BUFFER_LENGTH];

/*����3*/
volatile uint8_t  uart3_receive;
volatile uint16_t uart3_current_length;   
volatile uint8_t  uart3_buffer[UART3_BUFFER_LENGTH];

/*dma���͵�һ����*/
typedef struct {
#define BUFFER_MAX 3000 //������ÿɲ���
#define UART1_DMA_DEBUG 0
	volatile char buffer0[500];
	volatile char buffer1[2500];
	struct {
		volatile uint8_t buffer_state:1;
	}flags;
	volatile uint32_t ptr0;
	volatile uint32_t ptr1;
#if UART1_DMA_DEBUG==1
	/*����*/
	volatile uint32_t max_ptr0;
	volatile uint32_t max_ptr1;
#endif
}dma_trans_t;
static volatile dma_trans_t uart1_dma_trans;

/*�ڴ渴��*/
static void mem_cpy(volatile dma_trans_t* Struct, const char* src, uint32_t length) {

	if (Struct->flags.buffer_state==0){
		while (length--) {
			Struct->buffer0[Struct->ptr0++] = *src++;
		}
	}
	else {
		while (length--) {
			Struct->buffer1[Struct->ptr1++] = *src++;
		}
	}
}

//����1��ʼ��
void USART1_Config(uint32_t Baud_rate) 		//��ʼ�� ����USART1
{
	GPIO_InitTypeDef     GPIO_InitStructure;   //���ڶ˿����ýṹ�����
	USART_InitTypeDef    USART_InitStructure;  //���ڲ������ýṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//�򿪴��ڸ���ʱ��
	USART_DeInit(USART1);  //��λ����1

	//��2������USART1 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //����1���ͽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //����ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //��ʼ��GPIOA
														  
	//��3������USART Rx�����սţ���GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   //����1���ս�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //��ʼ��GPIOA

	//��4��������USART1����
	USART_InitStructure.USART_BaudRate             = Baud_rate;							 //���������ã�115200
	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;			 //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits             = USART_StopBits_1;				 //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity               = USART_Parity_No;				 //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None; //Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	 //�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);										 //��ʼ��USART1

  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART1�����ж� ��
    NVIC_EnableIRQ(USART1_IRQn);
    
	//��5����ʹ�� USART1�� �������
	USART_Cmd(USART1, ENABLE);							   //ʹ�� USART1

	/*dma����*/
	//��Ϣ:DMA1ͨ��4��USART1_TX, DMA1ͨ��5��USART1_RX.

	USART1->CR3|=1<<7; //ʹ�ܴ��ڵ�DMA����.
	DMA1_Channel4->CPAR=(uint32_t)&USART1->DR; //��������Ĵ�����ַ
	DMA1_Channel4->CCR|=1<<12;//����ͨ�����ȼ�,��
	DMA1_Channel4->CCR|=1<<4;//�Ӵ洢����������
	DMA1_Channel4->CCR|=1<<7;//Դ��ַ��,���費��.
	DMA1_Channel4->CCR|=1<<1;//���������ж�.
	//����m2m
	//��ѭ��
	//Ŀ�ĺ�Դ���ݿ��Ĭ��Ϊ8bit.
	__NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority (NVIC_PriorityGroup_4,1 ,0));
	__NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    //�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    //USART_ClearFlag(USART1, USART_FLAG_TC);                //�崮��1���ͱ�־
}


//����2��ʼ��
void USART2_Config(uint32_t Baud_rate)   //��ʼ�� ����USART2
{
	GPIO_InitTypeDef    GPIO_InitStructure;	   //���ڶ˿����ýṹ�����
	USART_InitTypeDef   USART_InitStructure;   //���ڲ������ýṹ�����

	//ʹ�� USART2 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//�򿪴��ڸ���ʱ��

	//��USART2 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //ѡ���ĸ�IO�� ��ѡ��PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //��ʼ��GPIOA

	//��USART2 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //ѡ���ĸ�IO�� ��ѡ��PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART2����
	USART_InitStructure.USART_BaudRate = Baud_rate;	                    //���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART2, &USART_InitStructure);                       //��ʼ��USART2
	
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART2�����ж�
    NVIC_EnableIRQ(USART2_IRQn);
    
	//ʹ�� USART2�� �������
	USART_Cmd(USART2, ENABLE);                             // USART2ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART2, USART_FLAG_TC);                //�崮��2���ͱ�־
}

//����3��ʼ��
void USART3_Config(uint32_t Baud_rate)   //��ʼ�� ����USART3
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;	  //���ڲ������ýṹ�����

	//ʹ�� USART3 ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//�򿪴��ڸ���ʱ��
  	
	//��USART3 Tx�����ͽţ���GPIO����Ϊ���츴��ģʽ   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //ѡ���ĸ�IO�� ��ѡ��PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //�趨IO�ڵ�����ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //��ʼ��GPIOB
	
	//��USART3 Rx�����սţ���GPIO����Ϊ��������ģʽ														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //ѡ���ĸ�IO�� ��ѡ��PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	          //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //��ʼ��GPIOA
	  
	//����USART3����
	USART_InitStructure.USART_BaudRate = Baud_rate;	                    //���������ã�2400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                       //��ʼ��USART3
	
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ��ָ����USART3�����ж�
    NVIC_EnableIRQ(USART3_IRQn);

	//ʹ�� USART3�� �������
	USART_Cmd(USART3, ENABLE);                             // USART3ʹ��

	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
    USART_ClearFlag(USART3, USART_FLAG_TC);                //�崮��3���ͱ�־
}

//���ڷ��ͺ���
void uart_transmit(USART_TypeDef * USARTx, uint8_t *ptr,uint16_t length )
{
    for(uint16_t i=0;i<length;i++)
    {
        USARTx->DR =ptr[i];
        while (!(USARTx->SR & 0X40));                  
    }
}

/*���ڷ��ַ���*/
void uart_transmit_string(USART_TypeDef *USARTx, const char *str)
{
    while(*str != '\0')  // ����ֱ������ֹ��
    {
        USARTx->DR = *str++;  // �������ݲ��ƶ�ָ��
        while (!(USARTx->SR & 0x40));  // �ȴ�������ɱ�־(TC)
    }
}

/*dma���ڷ���*/
void uart1_transmit_dma(const char *str,uint32_t length) {
	//�������û����
	if (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
		//���������ݴ滺����
		mem_cpy(&uart1_dma_trans, str, length);
		USART1->CR1 |= 1<<6;//��TC����ж�
	}
	else {
		DMA1_Channel4->CMAR = (uint32_t)str;
		DMA1_Channel4->CNDTR=length ; //���������Ҫ�ں�����ʹ��,��������,���0xffff
		DMA1_Channel4->CCR|=1; //enable
	}
}

#if UART1_DMA_DEBUG==1
/*��ӡ������ָ�����ֵ*/
void print_level(void) {
	char string[25];
	sprintf(string ,"max_ptr0:%d,max_ptr1:%d\r\n",(int)uart1_dma_trans.max_ptr0,(int)uart1_dma_trans.max_ptr1);
	uart1_transmit_dma(string,strlen(string));
}
#endif

//����1����
void USART1_IRQHandler(void)
{
	 /*rx����*/
    if(USART1->SR&0x20) //�����ж�
    {
        uint8_t temp= USART1->DR;
        uart1_buffer[uart1_current_length++]=temp;
        if(temp=='J')
        {
             uart1_receive=1;//���յ���
        }
    }
	/*tc����ж�*/
	if (USART1->SR&0x40) {
		//USART1->SR&=~0x40;//��tc����ж�
		if (uart1_dma_trans.flags.buffer_state==0) {
			DMA1_Channel4->CMAR = (uint32_t)uart1_dma_trans.buffer0;//Դ��ַ
			DMA1_Channel4->CNDTR=uart1_dma_trans.ptr0 ; //���������Ҫ�ں�����ʹ��,��������,���0xffff
#if UART1_DMA_DEBUG==1
			if (uart1_dma_trans.ptr0>=uart1_dma_trans.max_ptr0) {
				uart1_dma_trans.max_ptr0=uart1_dma_trans.ptr0;
			}
#endif
			uart1_dma_trans.ptr0=0;//����ָ���0
		}
		else {
			DMA1_Channel4->CMAR = (uint32_t)uart1_dma_trans.buffer1;//Դ��ַ
			DMA1_Channel4->CNDTR=uart1_dma_trans.ptr1 ; //���������Ҫ�ں�����ʹ��,��������,���0xffff
#if UART1_DMA_DEBUG==1
			if (uart1_dma_trans.ptr1>=uart1_dma_trans.max_ptr1) {
				uart1_dma_trans.max_ptr1=uart1_dma_trans.ptr1;
			}
#endif
			uart1_dma_trans.ptr1=0;//����ָ���0
		}
		uart1_dma_trans.flags.buffer_state=~uart1_dma_trans.flags.buffer_state;//�л�����
		//�����ܱ�֤������ڴ��������������ݲ����ƻ�.
		DMA1_Channel4->CCR|=1; //enable
		USART1->CR1 &= ~(1<<6);//��TC����ж�
	}
}

////����2����
//void USART2_IRQHandler(void)
//{
//    if(USART2->SR&0x20) //�����ж�
//    {
//        uint8_t temp= USART2->DR;
//        uart2_buffer[uart2_current_length++]=temp;
//        if(temp=='J')
//        {
//             uart2_receive=1;//���յ���
//        }        
//    }        
//}

////����3����
//void USART3_IRQHandler(void)
//{
//    if(USART3->SR&0x20) //�����ж�
//    {
//        uint8_t temp= USART3->DR;
//        uart3_buffer[uart3_current_length++]=temp;
//        if(temp=='J')
//        {
//             uart3_receive=1;//���յ���
//        }        
//    }        
//}

#if defined ( __GNUC__ )
/*sprintf��Ҫ*/
extern int _end;
caddr_t _sbrk(int incr)
{
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;
	if (heap == NULL)
	{
		heap = (unsigned char *) &_end;
	}
	prev_heap = heap;
	heap += incr;
	return (caddr_t) prev_heap;
}
#endif

/*����1��DMA�����ж�*/
void DMA1_Channel4_IRQHandler(void) {
	//��ʱdma������
	if (DMA1->ISR & DMA1_FLAG_TC4) {
		DMA1->IFCR = DMA1_FLAG_TC4;//���־
		DMA1_Channel4->CCR&=~1;  //�ر�dma
	}
}