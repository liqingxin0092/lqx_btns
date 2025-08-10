#ifndef __UART_H
#define __UART_H

#include "stdint.h"
#include "stm32f10x.h"


#define UARTx USART1

#define UART1_BUFFER_LENGTH 100
#define UART2_BUFFER_LENGTH 100
#define UART3_BUFFER_LENGTH 100

#define UART1_TAKE() do{       \
    uart1_receive=0;            \
    uart1_current_length=0;     \
}while(0)    


#define UART2_TAKE() do{    \
    uart2_receive=0;         \
    uart2_current_length=0;  \
}while(0)  

#define UART3_TAKE() do{    \
    uart3_receive=0;         \
    uart3_current_length=0;  \
}while(0)

extern volatile uint8_t uart1_receive;
extern volatile uint16_t uart1_current_length;
extern volatile uint8_t  uart1_buffer[UART1_BUFFER_LENGTH];

extern volatile uint8_t uart2_receive;
extern volatile uint16_t uart2_current_length;
extern volatile uint8_t  uart2_buffer[UART2_BUFFER_LENGTH];

extern volatile uint8_t uart3_receive;
extern volatile uint16_t uart3_current_length;
extern volatile uint8_t  uart3_buffer[UART3_BUFFER_LENGTH];

void USART1_Config(uint32_t Baud_rate);
void USART2_Config(uint32_t Baud_rate);
void USART3_Config(uint32_t Baud_rate);

void uart_transmit(USART_TypeDef * USARTx, uint8_t *ptr,uint16_t length );
void uart_transmit_string(USART_TypeDef *USARTx, const char *str);

void uart1_transmit_dma(const char *str,uint32_t length);

#if UART1_DMA_DEBUG==1
void print_level(void);
#endif

#endif


