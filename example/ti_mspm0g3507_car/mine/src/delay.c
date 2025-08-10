#include "delay.h"
#include "mspm0g350x.h"
#include "lqx_btns.h"
#include "ti_msp_dl_config.h"
#include "hmi.h"
#include "motor.h"

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks =  80*nus;
    told = SysTick->VAL;                    /* 刚进入时的计数器值 */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }
}

/**
 * @brief     延时nms
 * @param     nms: 要延时的ms数 (0< nms <= (2^32 / fac_us / 1000))(fac_us一般等于系统主频, 自行套入计算)
 * @retval    无
 */
void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000));                   /* 普通方式延时 */
}

extern struct 
{
    uint8_t is_start:1;
    uint32_t cnt;
}timer;

void SysTick_Handler(void)
{
#define KEY_DETECT_TIME 10
    static int8_t num= KEY_DETECT_TIME;
    if(--num==0)
    {
        num =KEY_DETECT_TIME;
        key_detect();
    }
    
    /*小车延时启动*/
    #define TIME  1000
    if(car_delay_start_timer.is_start==1)
    {
        if(++car_delay_start_timer.cnt==TIME)
        {
            set_gloab_speed(1);    //给速度
            car_delay_start_timer.cnt=0;
            car_delay_start_timer.is_start=0;
        }
    }

#if 0    
#define KEY_DETECT_TIME1 6000
    #include "jy61p.h"
    static int16_t num1= KEY_DETECT_TIME1;
    static uint8_t flag=0;
    if(--num1==0)
    {
        num1 =KEY_DETECT_TIME1;
        if(flag==0)
        {
            flag=~flag;
            jy61p_pid_struct.tar_val=-180;
        }
        else 
        {
            flag=~flag;
            jy61p_pid_struct.tar_val=0;
        }
    }
#endif
}
