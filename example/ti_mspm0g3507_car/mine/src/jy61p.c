#include "jy61p.h"
#include "ti_msp_dl_config.h"
#include "delay.h"

jy61p_flags_t jy61p_flags;
uint8_t jy61p_buffer[8];
uint8_t jy61p_buffer_ptr;
uint16_t angle_temp[3];

//-180~180 -> -0.5~ 0.5
pid_t jy61p_pid_struct=
{
   /* kp */
     .kp= 0.005,
   /* ki */
     .ki= 0.000,
   /* kd */
     .kd=0.0005,
   /* tar_val */
     .tar_val= 0,
   /* 积分误差累计值上限*/
     .inc_max= 0,
   /* 积分误差累计值下限*/
     .inc_min= 0,
   /* 积分加入的阈值*/
     .inc_use_threshold= 40,
   /* 输出值上限 */
     .out_max=40,
   /* 输出值下限 */
     .out_min= 0,
   /* 微分项过去权重 */
     .dpart=0,
   /* 输出偏移 */
     .out_offset= 0,
   /* 输出死区(误差容忍) */
     .out_threshold= 0,
     
     .data=(void *)1,
};

static void uart_send(const uint8_t* buf ,uint8_t num)
{
     for(uint8_t i=0;i<num;i++)
     {
         while(DL_UART_isTXFIFOFull(UART_3_INST)==true);
         DL_UART_Main_transmitData(UART_3_INST,buf[i]);   
     }
}

void wit_wrtite_reg(uint32_t Reg,uint32_t val)
{
    uint8_t buf[5]={0xff,0xaa};
    /*解锁*/
    buf[2] = 0x69;
    buf[3] = 0x88;
    buf[4] = 0xb5;
    uart_send(buf, 5);
    delay_ms(200);
    
    /*实际内容*/
    buf[2] = Reg;
    buf[3] = val&0xff;
    buf[4] = val>>8;
    uart_send(buf, 5);
    delay_ms(100);
    
    /*保存*/
    buf[2] = 0;
    buf[3] = 0;
    buf[4] = 0;
    uart_send(buf, 5);
}

void UART_3_INST_IRQHandler(void)
{
    //如果产生了串口中断
    switch( DL_UART_getPendingInterrupt(UART_3_INST))
    {
        char uart_data;
        case DL_UART_IIDX_RX://如果是接收中断 
            uart_data =UART_1_INST->RXDATA ;            
            jy61p_buffer[jy61p_buffer_ptr++]=uart_data;
            if(jy61p_buffer_ptr==2)
            {
                 if(jy61p_buffer[0]==0x55)//接收到包头 
                 {
                      switch (jy61p_buffer[1])
                      {
                          case 0x53: //角度
                               jy61p_flags.jy61p_content_type= 0x53;
                          break;
                          default: //没接收到类型代码
                              jy61p_buffer_ptr=0;
                          break;
                      }  
                 }
                 else  //没接收到包头
                 {
                      jy61p_buffer_ptr=0;
                 }
            }  
            if(jy61p_buffer_ptr==8)
            {
                if(jy61p_flags.jy61p_content_type==0x53)
                {
                    angle_temp[0]=jy61p_buffer[2]|(jy61p_buffer[3]<<8);
                    angle_temp[1]=jy61p_buffer[4]|(jy61p_buffer[5]<<8);
                    angle_temp[2]=jy61p_buffer[6]|(jy61p_buffer[7]<<8);
                }
                jy61p_buffer_ptr=0;  //指针复位
            }
            break;

        default://其他的串口中断
            break;
    }
}

float get_yaw(void)
{
     float temp= angle_temp[2]/32768.0f*180.0f;
     if(temp>180)
     {
        temp-=360; 
     }      
     return temp;
}