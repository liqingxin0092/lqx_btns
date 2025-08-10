#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include "ti_msp_dl_config.h"
#include "hash_cmd.h"
#include <stdio.h>

#define UART0_TAIL '\n'
#define UART0_BUFFER_LENGTH 100
#define UART0_0_OFFSET -2 

#define UART2_TAIL '\n'
#define UART2_BUFFER_LENGTH 100
#define UART2_0_OFFSET -2 

//这个放最后,要不数量被清了
#define UART0_RELEASE() do{       \
    uart0_receive=0;            \
    uart0_current_length=0;     \
}while(0)

#define UART2_RELEASE() do{       \
    uart2_receive=0;            \
    uart2_current_length=0;     \
}while(0)


//发字符
static inline void uart0_send_char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    while( DL_UART_isBusy(UART_0_INST) == true );
    //发送单个字符
    DL_UART_Main_transmitData(UART_0_INST, ch);
}

//串口发送字符串
static inline void uart0_send_string(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        uart0_send_char(*str++);
    }
}

void mine_uart_init(void);
void uart_process( void * pvParameters );
void UART_process(void);
void uart2_send(const uint8_t* buf,uint16_t length);

#endif 
