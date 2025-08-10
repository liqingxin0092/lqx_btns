#include "gray_scale_sensor.h"
#include "delay.h"

/*�ô�mcu�Ļ��ǲ�����*/
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
   /* ��������ۼ�ֵ����*/
     .inc_max= 0,
   /* ��������ۼ�ֵ����*/
     .inc_min= 0,
   /* ���ּ������ֵ*/
     .inc_use_threshold= 40,
   /* ���ֵ���� */
     .out_max=40,
   /* ���ֵ���� */
     .out_min= 0,
   /* ΢�����ȥȨ�� */
     .dpart=0,
   /* ���ƫ�� */
     .out_offset= 0,
   /* �������(�������) */
     .out_threshold= 0,
 };

volatile sensor_data_t sensor_bits;
#include "GUI.h"
#include <stdio.h>
#include "Lcd_Driver.h"
/*�Ƿ񴥷�*/
bool scale_is_trigger(void)
{
#define THRESHOLD 20
    bool temp=0;
    static int8_t num=0;
     /*��ֵ*/
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
/*��mcu����*/
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

#else   /* ����mcu���� */
 
/**************************��mcu�Ҷȴ�����**********************************/
#define DIRECTION 0 //����
#define BLACK_THRESHOLD 2000 //С�����������Ϊʶ�𵽺�ɫ
#define Switch_Address_0(val) (val)?(scale_PORT->DOUTSET31_0=scale_ad0_PIN):(scale_PORT->DOUTCLR31_0=scale_ad0_PIN)
#define Switch_Address_1(val) (val)?(scale_PORT->DOUTSET31_0=scale_ad1_PIN):(scale_PORT->DOUTCLR31_0=scale_ad1_PIN)
#define Switch_Address_2(val) (val)?(scale_PORT->DOUTSET31_0=scale_ad2_PIN):(scale_PORT->DOUTCLR31_0=scale_ad2_PIN)

static uint16_t Get_adc_of_user(void)
{   
    DL_ADC12_startConversion(ADC12_0_INST);
    return DL_ADC12_getMemResult(ADC12_0_INST, ADC12_0_ADCMEM_0);
}

/* �������ܣ��ɼ�8��ͨ����ģ��ֵ�����о�ֵ�˲�
   ����˵����result - �洢8��ͨ�������������� */
static void Get_Analog_value(unsigned short *result)
{
    unsigned char i,j;
    unsigned int Anolag=0;
    
    // ����8��������ͨ����3λ��ַ����ϣ�
    for(i=0;i<8;i++)
    {
        // ͨ����ַ������л�������ͨ����ע��ȡ���߼���
        Switch_Address_0(!(i&0x01));  // ��ַ��0����Ӧbit0
        Switch_Address_1(!(i&0x02));  // ��ַ��1����Ӧbit1
        Switch_Address_2(!(i&0x04));  // ��ַ��2����Ӧbit2

        // ÿ��ͨ���ɼ�8��ADCֵ���о�ֵ�˲�
        for(j=0;j<8;j++)
        {
            Anolag+=Get_adc_of_user(); 
        }        
#if DIRECTION == 1
            result[i]=Anolag/8;  // ����ƽ��ֵ
#else 
            result[7-i]=Anolag/8;  // ����ƽ��ֵ
#endif
        Anolag=0;  // �����ۼ���
    }
}

//��ɫ��1
//��ɫ��0
uint16_t temp[8];
void get_no_mcu_scale_data(sensor_data_t* data)
{
//    uint16_t temp[8]; //ԭʼ����
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

/*ת��Ϊpidʶ����*/
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
