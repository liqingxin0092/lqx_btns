#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"
#include "delay.h"
#include "lqx_btns.h"
#include "uart.h"


int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    __NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
    RCC->APB2ENR|=1;      //AFIO时钟
    AFIO->MAPR|= 0x2000000;  //解除jtag
    SysTick_Config(72000);//1ms

    key_init(); //按键初始化

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIOC->ODR|=1<<13;

    USART1_Config(115200);


    char string[50];

    for (uint16_t i=0;i<200;i++) {
        sprintf(string, "%dliqingxin\r\n",i);
        uart1_transmit_dma(string,strlen(string)) ;
        //delay_ms(1);
    }

   // print_level();
    uint16_t key_code;
    while(1)
    {
        if (get_key_event(&key_code)) {
            sprintf(string,"key:%d\r\n",key_code);
            uart1_transmit_dma(string,strlen(string));
            if (key_code==0) {
                uart1_transmit_dma("1246152\r\n",strlen("1246152\r\n")) ;
                uart1_transmit_dma("21555555555555555555\r\n",strlen("21555555555555555555\r\n")) ;
                uart1_transmit_dma("4486232\r\n",strlen("4486232\r\n")) ;
                uart1_transmit_dma("1abracadabra\r\n",strlen("1abracadabra\r\n")) ;
                //print_level();
            }
            else if (key_code==3) {
                uart1_transmit_dma("1246152\r\n",strlen("1246152\r\n")) ;
                uart1_transmit_dma("21555555555555555555\r\n",strlen("21555555555555555555\r\n")) ;
                uart1_transmit_dma("4486232\r\n",strlen("4486232\r\n")) ;
                uart1_transmit_dma("1abracadabra\r\n",strlen("1abracadabra\r\n")) ;
                //print_level();
            }
        }
    }

    return 0;
}



