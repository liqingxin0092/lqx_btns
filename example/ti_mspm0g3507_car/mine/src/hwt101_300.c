#include "hwt101_300.h"

uint8_t hwt101_buffer[12];  //陀螺仪原始数据
uint8_t hwt101_buffer_ptr; //上面的数组指针
uint16_t angle_temp;       //有用的中间产物
hwt_flags_t hwt_flags; 

/*陀螺仪串口发送*/
void uart_send(const uint8_t* buf ,uint8_t num)
{
     for(uint8_t i=0;i<num;i++)
     {
         while(DL_UART_isTXFIFOFull(HWT_UART)==true);
         DL_UART_Main_transmitData(HWT_UART,buf[i]);   
     }
}

/*数据接收*/
void HWT_UART_HANDLE(void)
{
    //如果产生了串口中断
    switch( DL_UART_getPendingInterrupt(HWT_UART) )
    {
        char uart_data;
        case DL_UART_IIDX_RX://如果是接收中断 
            uart_data =HWT_UART->RXDATA ;            
            hwt101_buffer[hwt101_buffer_ptr++]=uart_data;
            if(hwt101_buffer_ptr>=4)
            {
                if(hwt101_buffer[hwt101_buffer_ptr-1]==0x27&&hwt101_buffer[hwt101_buffer_ptr-2]==0xE5)
                {
                      angle_temp=*(uint16_t*)&hwt101_buffer[hwt101_buffer_ptr-4];
                      hwt101_buffer_ptr=0;
                }
            }
            if(hwt101_buffer_ptr>=11)
            {
                  hwt101_buffer_ptr=0;
            }
            break;

        default://其他的串口中断
            break;
    }
}

float hwt_get_yaw(void)
{
     float temp= angle_temp/32768.0f*180.0f;
     if(temp>180)
     {
        temp-=360; 
     }      
     return temp;
}