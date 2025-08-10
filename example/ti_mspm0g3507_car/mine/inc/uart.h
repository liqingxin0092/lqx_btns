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

//��������,Ҫ������������
#define UART0_RELEASE() do{       \
    uart0_receive=0;            \
    uart0_current_length=0;     \
}while(0)

#define UART2_RELEASE() do{       \
    uart2_receive=0;            \
    uart2_current_length=0;     \
}while(0)


//���ַ�
static inline void uart0_send_char(char ch)
{
    //������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
    while( DL_UART_isBusy(UART_0_INST) == true );
    //���͵����ַ�
    DL_UART_Main_transmitData(UART_0_INST, ch);
}

//���ڷ����ַ���
static inline void uart0_send_string(char* str)
{
    //��ǰ�ַ�����ַ���ڽ�β ���� �ַ����׵�ַ��Ϊ��
    while(*str!=0&&str!=0)
    {
        //�����ַ����׵�ַ�е��ַ��������ڷ������֮���׵�ַ����
        uart0_send_char(*str++);
    }
}

void mine_uart_init(void);
void uart_process( void * pvParameters );
void UART_process(void);
void uart2_send(const uint8_t* buf,uint16_t length);

#endif 
