#include "bsp.h"

extern uint16_t temp[8];

int main(void)
{
     
     SYSCFG_DL_init();
     mine_uart_init();
    
     /*�������õĴ���()*/
     NVIC_ClearPendingIRQ(UART_1_INST_INT_IRQN);
     NVIC_EnableIRQ(UART_1_INST_INT_IRQN); 
    
     /*�����������������*/
     NVIC_ClearPendingIRQ(soft_encoder_INT_IRQN);
     NVIC_EnableIRQ(soft_encoder_INT_IRQN);
    
     /*��c8t6ͨ���õĴ���*/  //�������115200,��β\r\n,-2.
     NVIC_ClearPendingIRQ(UART_2_INST_INT_IRQN);
     NVIC_EnableIRQ(UART_2_INST_INT_IRQN);
    
     /*pwm*/
     PWM_l_INST->COUNTERREGS.CTRCTL|= DL_TIMER_START;
    
     /*���ٶȶ�ʱ��*/
     encoder_INST->COUNTERREGS.CTRCTL|= DL_TIMER_START;  
     NVIC_ClearPendingIRQ(encoder_INST_INT_IRQN);
     NVIC_EnableIRQ(encoder_INST_INT_IRQN);
     
     Lcd_Init();
     key_init();
     
     set_gloab_speed(0);
     
     
     char string[50];
     while(1)
     {
        UART_process();     
        hmi_task();
      
        
        /* debug */
        //sprintf(string,"%d,%d,%d    ",temp[0],temp[1],temp[2]); 
        //sprintf(string,"%.2f,%.2f,%#x  ",car_instance.l_speed,car_instance.r_speed,sensor_bits.byte);
        //Gui_DrawFont_GBK16(0, 0, YELLOW,BLACK, (uint8_t*)string);
     }

     return 0;
}


