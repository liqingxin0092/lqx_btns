#include "uart.h"
#include <stdio.h>
#include "motor.h"
#include <string.h>

/*串口0*/
volatile uint8_t  uart0_receive;
volatile uint16_t uart0_current_length;    
volatile uint8_t  uart0_buffer[UART0_BUFFER_LENGTH];

/*串口2*/
volatile uint8_t  uart2_receive;
volatile uint16_t uart2_current_length;    
volatile uint8_t  uart2_buffer[UART2_BUFFER_LENGTH];
//跟c8t6通信:uart2_send(const uint8_t* buf,uint16_t length);

//串口的中断服务函数
void UART0_IRQHandler(void)
{
    //如果产生了串口中断
    switch( DL_UART_getPendingInterrupt(UART_0_INST) )
    {
        char uart_data;
        case DL_UART_IIDX_RX://如果是接收中断 
            uart_data =UART_0_INST->RXDATA ;
            uart0_buffer[uart0_current_length++]=uart_data;
            if(uart_data==UART0_TAIL)
            {
                 uart0_receive=1;//接收到了
                
            }  
            break;

        default://其他的串口中断
            break;
    }
}
    
void UART_2_INST_IRQHandler(void) 
{
    //如果产生了串口中断
    switch( DL_UART_getPendingInterrupt(UART_2_INST) )
    {
        char uart_data;
        case DL_UART_IIDX_RX://如果是接收中断 
            uart_data =UART_2_INST->RXDATA ;
            uart2_buffer[uart2_current_length++]=uart_data;
            if(uart_data==UART2_TAIL)
            {
                 uart2_receive=1;//接收到了                
            }  
            break;

        default://其他的串口中断
            break;
    }
}

void uart2_send(const uint8_t* buf,uint16_t length)
{
    for(uint16_t i=0;i<length;i++)
    {
        while(DL_UART_isTXFIFOFull(UART_2_INST)==true);
        DL_UART_Main_transmitData(UART_2_INST,buf[i]); 
    }
}

static void test_task(void)
{
    printf("test excute\r\n");  
    uart2_send((const uint8_t*)"A\r\n",strlen("a\r\n"));
}

static void conf_task(void)
{
    printf("conf excute\r\n");
    uart2_send((const uint8_t*)"B\r\n",strlen("b\r\n"));
}

static void abcd_task(void)
{
    printf("abcd excute\r\n");    
}

uint8_t car_shoud_stop;
void UART_process(void)
{
    /*debug*/
    if(uart0_receive)
    {
        uart0_buffer[uart0_current_length+UART0_0_OFFSET]='\0'; //包尾变字符串 
        
        execute_cmd((char*)uart0_buffer);
        
        UART0_RELEASE();
    }   
    
    /*跟从c8t6通信*/
    if(uart2_receive)
    {
        uart2_buffer[uart2_current_length+UART2_0_OFFSET]='\0'; //包尾变字符串 
        
//        sscanf(uart2_buffer,"%d",&car_shoud_stop);
        
        UART2_RELEASE();
    }     
}

void mine_uart_init(void)
{
    //清除串口中断标志
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    //使能串口中断
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    
    /*注册命令*/
    register_cmd("test",test_task);
    register_cmd("conf",conf_task);
    register_cmd("abcd",abcd_task);
}

int fputc(int ch, FILE *stream)
{
     while(DL_UART_isTXFIFOFull(UART_0_INST)==true);
     DL_UART_Main_transmitData(UART_0_INST,ch); 
     return ch;
}
