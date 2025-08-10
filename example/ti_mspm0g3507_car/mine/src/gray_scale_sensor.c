#include "gray_scale_sensor.h"
#include "delay.h"

/*用带mcu的还是不带的*/
#define WITH_MCU 0

pid_t scale_pid_struct=
{
   /* kp */
     .kp= 0.04,
   /* ki */
     .ki=0.000,
   /* kd */
     .kd=0.003,
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
 };

volatile sensor_data_t sensor_bits;
#include "GUI.h"
#include <stdio.h>
#include "Lcd_Driver.h"
/*是否触发*/
bool scale_is_trigger(void)
{
#define THRESHOLD 20
    bool temp=0;
    static int8_t num=0;
     /*赋值*/
     if((0<sensor_bits.byte)&&(sensor_bits.byte<0xff))
     {
        if(++num>=THRESHOLD) 
        {
            num=THRESHOLD;
            temp=true;
        }
        else 
        {
            temp=0;
        }
     }        
     else 
     {
        if(--num<=0)
            num=0; 
        temp=0;
     }        
     return temp;
}

#if WITH_MCU ==1
/*带mcu处理*/
void read_gray_scale_sensor(volatile sensor_data_t* instance)
{
    for(uint8_t i=0;i<8;i++)
    {
        gray_scale_PORT->DOUTCLR31_0=gray_scale_clk_pin_PIN; //clk 0
        if(gray_scale_PORT->DIN31_0&gray_scale_dat_pin_PIN)
             (*instance).byte|=1<<i;
        else 
             (*instance).byte&=~(1<<i);  
        gray_scale_PORT->DOUTSET31_0=gray_scale_clk_pin_PIN; //clk 1
        delay_us(5);
    }  
    delay_ms(1);
}

#else   /* 不带mcu处理 */
 
/**************************无mcu灰度传感器**********************************/
#define DIRECTION 0 //方向
#define BLACK_THRESHOLD 2000 //小于这个数被认为识别到黑色
#define Switch_Address_0(val) (val)?(scale_PORT->DOUTSET31_0=scale_ad0_PIN):(scale_PORT->DOUTCLR31_0=scale_ad0_PIN)
#define Switch_Address_1(val) (val)?(scale_PORT->DOUTSET31_0=scale_ad1_PIN):(scale_PORT->DOUTCLR31_0=scale_ad1_PIN)
#define Switch_Address_2(val) (val)?(scale_PORT->DOUTSET31_0=scale_ad2_PIN):(scale_PORT->DOUTCLR31_0=scale_ad2_PIN)

static uint16_t Get_adc_of_user(void)
{   
    DL_ADC12_startConversion(ADC12_0_INST);
    return DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);
}

/* 函数功能：采集8个通道的模拟值并进行均值滤波
   参数说明：result - 存储8个通道处理结果的数组 */
static void Get_Analog_value(unsigned short *result)
{
    unsigned char i,j;
    unsigned int Anolag=0;
    
    // 遍历8个传感器通道（3位地址线组合）
    for(i=0;i<8;i++)
    {
        // 通过地址线组合切换传感器通道（注意取反逻辑）
        Switch_Address_0(!(i&0x01));  // 地址线0，对应bit0
        Switch_Address_1(!(i&0x02));  // 地址线1，对应bit1
        Switch_Address_2(!(i&0x04));  // 地址线2，对应bit2

        // 每个通道采集8次ADC值进行均值滤波
        for(j=0;j<8;j++)
        {
            Anolag+=Get_adc_of_user(); 
        }        
#if DIRECTION == 1
            result[i]=Anolag/8;  // 计算平均值
#else 
            result[7-i]=Anolag/8;  // 计算平均值
#endif
        Anolag=0;  // 重置累加器
    }
}

//白色置1
//黑色置0
uint16_t temp[8];
void get_no_mcu_scale_data(sensor_data_t* data)
{
//    uint16_t temp[8]; //原始数据
    Get_Analog_value(temp);
    for (int i = 0; i < 8; i++) 
    {
        if(temp[i]> BLACK_THRESHOLD)
             data->byte|=1<<i;
        else 
             data->byte&=~(1<<i);
    }
}
#endif

/*转换为pid识别量*/
int8_t scale_val(const sensor_data_t* data)
{
    int8_t val=0;
    for(uint8_t i=0;i<8;i++)
    {
        if(!(data->byte&(1<<i)))
        {
             switch(i)
             {
                 case 0:
                       val+=8;
                    break;
                 case 1:
                       val+=4;
                     break;
                 case 2:
                       val+=2;
                     break;
                 case 5:
                       val-=2;
                     break;
                 case 6:
                       val-=4;
                     break;
                 case 7:
                       val-=8;
                     break;   
             }
        }
    }  
    return val;    
}
