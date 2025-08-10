#include "uart.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "stm32f10x.h"

/*串口1*/
volatile uint8_t uart1_receive;
volatile uint16_t uart1_current_length;    
volatile uint8_t uart1_buffer[UART1_BUFFER_LENGTH];

/*串口2*/
volatile uint8_t  uart2_receive;
volatile uint16_t uart2_current_length;   
volatile uint8_t  uart2_buffer[UART2_BUFFER_LENGTH];

/*串口3*/
volatile uint8_t  uart3_receive;
volatile uint16_t uart3_current_length;   
volatile uint8_t  uart3_buffer[UART3_BUFFER_LENGTH];

/*dma发送的一个类*/
typedef struct {
#define BUFFER_MAX 3000 //这个可用可不用
#define UART1_DMA_DEBUG 0
	volatile char buffer0[500];
	volatile char buffer1[2500];
	struct {
		volatile uint8_t buffer_state:1;
	}flags;
	volatile uint32_t ptr0;
	volatile uint32_t ptr1;
#if UART1_DMA_DEBUG==1
	/*调试*/
	volatile uint32_t max_ptr0;
	volatile uint32_t max_ptr1;
#endif
}dma_trans_t;
static volatile dma_trans_t uart1_dma_trans;

/*内存复制*/
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

//串口1初始化
void USART1_Config(uint32_t Baud_rate) 		//初始化 配置USART1
{
	GPIO_InitTypeDef     GPIO_InitStructure;   //串口端口配置结构体变量
	USART_InitTypeDef    USART_InitStructure;  //串口参数配置结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//打开串口复用时钟
	USART_DeInit(USART1);  //复位串口1

	//第2步：将USART1 Tx（发送脚）的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			   //串口1发送脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //输出速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA
														  
	//第3步：将USART Rx（接收脚）的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   //串口1接收脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   //初始化GPIOA

	//第4步：配置USART1参数
	USART_InitStructure.USART_BaudRate             = Baud_rate;							 //波特率设置：115200
	USART_InitStructure.USART_WordLength           = USART_WordLength_8b;			 //数据位数设置：8位
	USART_InitStructure.USART_StopBits             = USART_StopBits_1;				 //停止位设置：1位
	USART_InitStructure.USART_Parity               = USART_Parity_No;				 //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl  = USART_HardwareFlowControl_None; //硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode                 = USART_Mode_Rx | USART_Mode_Tx;	 //接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);										 //初始化USART1

  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能指定的USART1接收中断 ；
    NVIC_EnableIRQ(USART1_IRQn);
    
	//第5步：使能 USART1， 配置完毕
	USART_Cmd(USART1, ENABLE);							   //使能 USART1

	/*dma设置*/
	//信息:DMA1通道4是USART1_TX, DMA1通道5是USART1_RX.

	USART1->CR3|=1<<7; //使能串口的DMA发送.
	DMA1_Channel4->CPAR=(uint32_t)&USART1->DR; //设置外设寄存器地址
	DMA1_Channel4->CCR|=1<<12;//设置通达优先级,中
	DMA1_Channel4->CCR|=1<<4;//从存储器读到外设
	DMA1_Channel4->CCR|=1<<7;//源地址增,外设不增.
	DMA1_Channel4->CCR|=1<<1;//开传输完中断.
	//不用m2m
	//不循环
	//目的和源数据宽度默认为8bit.
	__NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority (NVIC_PriorityGroup_4,1 ,0));
	__NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    //如下语句解决第1个字节无法正确发送出去的问题
    //USART_ClearFlag(USART1, USART_FLAG_TC);                //清串口1发送标志
}


//串口2初始化
void USART2_Config(uint32_t Baud_rate)   //初始化 配置USART2
{
	GPIO_InitTypeDef    GPIO_InitStructure;	   //串口端口配置结构体变量
	USART_InitTypeDef   USART_InitStructure;   //串口参数配置结构体变量

	//使能 USART2 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//打开串口复用时钟

	//将USART2 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				  //选定哪个IO口 现选定PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    				  //初始化GPIOA

	//将USART2 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				  //选定哪个IO口 现选定PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART2参数
	USART_InitStructure.USART_BaudRate = Baud_rate;	                    //波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART2, &USART_InitStructure);                       //初始化USART2
	
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能指定的USART2接收中断
    NVIC_EnableIRQ(USART2_IRQn);
    
	//使能 USART2， 配置完毕
	USART_Cmd(USART2, ENABLE);                             // USART2使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(USART2, USART_FLAG_TC);                //清串口2发送标志
}

//串口3初始化
void USART3_Config(uint32_t Baud_rate)   //初始化 配置USART3
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;	  //串口参数配置结构体变量

	//使能 USART3 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//打开串口复用时钟
  	
	//将USART3 Tx（发送脚）的GPIO配置为推挽复用模式   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				  //选定哪个IO口 现选定PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		  //设定IO口的输出速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);    				  //初始化GPIOB
	
	//将USART3 Rx（接收脚）的GPIO配置为浮空输入模式														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				  //选定哪个IO口 现选定PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	          //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                    //初始化GPIOA
	  
	//配置USART3参数
	USART_InitStructure.USART_BaudRate = Baud_rate;	                    //波特率设置：2400
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	    //数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	        //停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;            //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收与发送都使能
	USART_Init(USART3, &USART_InitStructure);                       //初始化USART3
	
  	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 使能指定的USART3接收中断
    NVIC_EnableIRQ(USART3_IRQn);

	//使能 USART3， 配置完毕
	USART_Cmd(USART3, ENABLE);                             // USART3使能

	//如下语句解决第1个字节无法正确发送出去的问题
    USART_ClearFlag(USART3, USART_FLAG_TC);                //清串口3发送标志
}

//串口发送函数
void uart_transmit(USART_TypeDef * USARTx, uint8_t *ptr,uint16_t length )
{
    for(uint16_t i=0;i<length;i++)
    {
        USARTx->DR =ptr[i];
        while (!(USARTx->SR & 0X40));                  
    }
}

/*串口发字符串*/
void uart_transmit_string(USART_TypeDef *USARTx, const char *str)
{
    while(*str != '\0')  // 遍历直到空终止符
    {
        USARTx->DR = *str++;  // 加载数据并移动指针
        while (!(USARTx->SR & 0x40));  // 等待发送完成标志(TC)
    }
}

/*dma串口发送*/
void uart1_transmit_dma(const char *str,uint32_t length) {
	//如果串口没发完
	if (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
		//本次内容暂存缓冲区
		mem_cpy(&uart1_dma_trans, str, length);
		USART1->CR1 |= 1<<6;//开TC完成中断
	}
	else {
		DMA1_Channel4->CMAR = (uint32_t)str;
		DMA1_Channel4->CNDTR=length ; //这个可能需要在函数中使用,设置数量,最大0xffff
		DMA1_Channel4->CCR|=1; //enable
	}
}

#if UART1_DMA_DEBUG==1
/*打印缓冲区指针最大值*/
void print_level(void) {
	char string[25];
	sprintf(string ,"max_ptr0:%d,max_ptr1:%d\r\n",(int)uart1_dma_trans.max_ptr0,(int)uart1_dma_trans.max_ptr1);
	uart1_transmit_dma(string,strlen(string));
}
#endif

//串口1接收
void USART1_IRQHandler(void)
{
	 /*rx接收*/
    if(USART1->SR&0x20) //有数中断
    {
        uint8_t temp= USART1->DR;
        uart1_buffer[uart1_current_length++]=temp;
        if(temp=='J')
        {
             uart1_receive=1;//接收到了
        }
    }
	/*tc完成中断*/
	if (USART1->SR&0x40) {
		//USART1->SR&=~0x40;//清tc完成中断
		if (uart1_dma_trans.flags.buffer_state==0) {
			DMA1_Channel4->CMAR = (uint32_t)uart1_dma_trans.buffer0;//源地址
			DMA1_Channel4->CNDTR=uart1_dma_trans.ptr0 ; //这个可能需要在函数中使用,设置数量,最大0xffff
#if UART1_DMA_DEBUG==1
			if (uart1_dma_trans.ptr0>=uart1_dma_trans.max_ptr0) {
				uart1_dma_trans.max_ptr0=uart1_dma_trans.ptr0;
			}
#endif
			uart1_dma_trans.ptr0=0;//数组指针归0
		}
		else {
			DMA1_Channel4->CMAR = (uint32_t)uart1_dma_trans.buffer1;//源地址
			DMA1_Channel4->CNDTR=uart1_dma_trans.ptr1 ; //这个可能需要在函数中使用,设置数量,最大0xffff
#if UART1_DMA_DEBUG==1
			if (uart1_dma_trans.ptr1>=uart1_dma_trans.max_ptr1) {
				uart1_dma_trans.max_ptr1=uart1_dma_trans.ptr1;
			}
#endif
			uart1_dma_trans.ptr1=0;//数组指针归0
		}
		uart1_dma_trans.flags.buffer_state=~uart1_dma_trans.flags.buffer_state;//切换数组
		//尽可能保证这个正在传输的数组里的内容不被破坏.
		DMA1_Channel4->CCR|=1; //enable
		USART1->CR1 &= ~(1<<6);//关TC完成中断
	}
}

////串口2接收
//void USART2_IRQHandler(void)
//{
//    if(USART2->SR&0x20) //有数中断
//    {
//        uint8_t temp= USART2->DR;
//        uart2_buffer[uart2_current_length++]=temp;
//        if(temp=='J')
//        {
//             uart2_receive=1;//接收到了
//        }        
//    }        
//}

////串口3接收
//void USART3_IRQHandler(void)
//{
//    if(USART3->SR&0x20) //有数中断
//    {
//        uint8_t temp= USART3->DR;
//        uart3_buffer[uart3_current_length++]=temp;
//        if(temp=='J')
//        {
//             uart3_receive=1;//接收到了
//        }        
//    }        
//}

#if defined ( __GNUC__ )
/*sprintf需要*/
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

/*串口1的DMA发送中断*/
void DMA1_Channel4_IRQHandler(void) {
	//此时dma发完了
	if (DMA1->ISR & DMA1_FLAG_TC4) {
		DMA1->IFCR = DMA1_FLAG_TC4;//清标志
		DMA1_Channel4->CCR&=~1;  //关闭dma
	}
}