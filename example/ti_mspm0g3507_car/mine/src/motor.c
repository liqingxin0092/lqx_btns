#include "motor.h"
#include "gray_scale_sensor.h"
#include "jy61p.h"
#include "hmi.h"
#include "uart.h"

ENCODER_RES motor_encoder;

pid_t left_pid_struct=
{
   /* kp */
     .kp=1600,
   /* ki */
     .ki=15,
   /* kd */
     .kd=20,
   /* tar_val */
     .tar_val= 0,
   /* ��������ۼ�ֵ����*/
     .inc_max= 0,
   /* ��������ۼ�ֵ����*/
     .inc_min= 0,
   /* ���ּ������ֵ*/
     .inc_use_threshold= 1,
   /* ���ֵ���� */
     .out_max=1999,
   /* ���ֵ���� */
     .out_min= -1999,
   /* ΢�����ȥȨ�� */
     .dpart=0,
   /* ���ƫ�� */
     .out_offset= 0,
   /* �������(�������) */
     .out_threshold= 0,
};

pid_t right_pid_struct=
{
   /* kp */
     .kp=1600,
   /* ki */
     .ki=15,
   /* kd */
     .kd=20,
   /* tar_val */
     .tar_val= 0,
   /* ��������ۼ�ֵ����*/
     .inc_max= 0,
   /* ��������ۼ�ֵ����*/
     .inc_min= 0,
   /* ���ּ������ֵ*/
     .inc_use_threshold= 1,
   /* ���ֵ���� */
     .out_max=1999,
   /* ���ֵ���� */
     .out_min= -1999,
   /* ΢�����ȥȨ�� */
     .dpart=0,
   /* ���ƫ�� */
     .out_offset= 0,
   /* �������(�������) */
     .out_threshold= 0,
};

//���������ݸ���
//����һ��ʱ�����
void encoder_update(void)
{
	motor_encoder.count = motor_encoder.temp_count;

	//ȷ������
	motor_encoder.dir = ( motor_encoder.count >= 0 ) ? FORWARD : REVERSAL;

}

//�ⲿ�жϴ�����
void GROUP1_IRQHandler(void)
{
	uint32_t gpio_status;

	//��ȡ�ж��ź����
	gpio_status = DL_GPIO_getEnabledInterruptStatus(soft_encoder_PORT, soft_encoder_PIN_A_PIN | soft_encoder_PIN_B_PIN);
	//������A�������ش���
	if((gpio_status & soft_encoder_PIN_A_PIN) == soft_encoder_PIN_A_PIN)
	{
		//�����A���������£�B��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(soft_encoder_PORT,soft_encoder_PIN_B_PIN))
		{
			motor_encoder.temp_count--;
		}
		else
		{
			motor_encoder.temp_count++;
		}
	}//������B�������ش���
	else if((gpio_status & soft_encoder_PIN_B_PIN)==soft_encoder_PIN_B_PIN)
	{
		//�����B���������£�A��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(soft_encoder_PORT,soft_encoder_PIN_A_PIN))
		{
			motor_encoder.temp_count++;
		}
		else
		{
			motor_encoder.temp_count--;
		}
	}
	//���״̬
	DL_GPIO_clearInterruptStatus(soft_encoder_PORT,soft_encoder_PIN_A_PIN|soft_encoder_PIN_B_PIN);
}



/*-1999~1999*/
void _l_power(int16_t power)
{
     if(power>0)
     {                    
         car_l_out_0_PORT->DOUTCLR31_0=car_l_out_0_PIN;  //0
         car_l_out_1_PORT->DOUTSET31_0=car_l_out_1_PIN;  //1
         DL_TimerG_setCaptureCompareValue(PWM_l_INST,power,GPIO_PWM_l_C0_IDX);
     }
     else if(power==0)
     {
         car_l_out_0_PORT->DOUTCLR31_0=car_l_out_0_PIN;  //0
         car_l_out_1_PORT->DOUTCLR31_0=car_l_out_1_PIN;  //1
         DL_TimerG_setCaptureCompareValue(PWM_l_INST,0,GPIO_PWM_l_C0_IDX);
     }
     else
     {
         car_l_out_0_PORT->DOUTSET31_0=car_l_out_0_PIN;  //1
         car_l_out_1_PORT->DOUTCLR31_0=car_l_out_1_PIN;  //0
         DL_TimerG_setCaptureCompareValue(PWM_l_INST,-power,GPIO_PWM_l_C0_IDX);
     }            
}

/*-1999~1999*/
void _r_power(int16_t power)
{
     if(power>0)
     {                    
         car_r_out_0_PORT->DOUTCLR31_0=car_r_out_0_PIN;  //0
         car_r_out_1_PORT->DOUTSET31_0=car_r_out_1_PIN;  //1
         DL_TimerG_setCaptureCompareValue(PWM_l_INST,power,GPIO_PWM_l_C1_IDX);
     }
     else if(power==0)
     {
         car_r_out_0_PORT->DOUTCLR31_0=car_r_out_0_PIN;  //0
         car_r_out_1_PORT->DOUTCLR31_0=car_r_out_1_PIN;  //1
         DL_TimerG_setCaptureCompareValue(PWM_l_INST,0,GPIO_PWM_l_C1_IDX); 
     }
     else
     {
         car_r_out_0_PORT->DOUTSET31_0=car_r_out_0_PIN;  //1
         car_r_out_1_PORT->DOUTCLR31_0=car_r_out_1_PIN;  //0
         DL_TimerG_setCaptureCompareValue(PWM_l_INST,-power,GPIO_PWM_l_C1_IDX);
     }            
}

static void _set_speed(float l_s,float r_s)
{
    left_pid_struct  .tar_val= l_s;
    right_pid_struct .tar_val= r_s;
}
car_t car_instance=
{
    .give_l_power =  _l_power,
    .give_r_power =  _r_power,
    .set_speed    =  _set_speed,
};

/*debug:pid���������*/
float l_out,r_out;

static void change_tar_angle(void)
{
    static uint8_t flag=0;
    if(flag==0)
    {
        flag=~flag;
//        jy61p_pid_struct.tar_val=-180;
    }
    else
    {
        flag=~flag;
//        jy61p_pid_struct.tar_val=0;
    }
}

/*������ʧǿ��ת��*/
//����:��ת,������ת
#define ADD_SPEED 1

/*ѭ��ģʽ*/
#define START_TRACKING 1

/*·��ͳ��*/
#define USING_ENCODER_SUM 0

/*������ʧǿ��ת��*/
#define FORCE_TURN 0  //ֱ��ת��,�������Ǹ�����
#define NO_STD_TURN 1 //��ֱ��ת��
#define CNT_LENGTH 100  // 100 * 20 ��������һ�ιյ�
#define STRAIGHT_AHEAD_TIME 30 //��⵽�յ��ֱ�ߵ�ʱ��
#define TURE_LEFT_TIME 40      //��תʱ��

struct
{
    uint8_t is_start:1;
    uint32_t cnt;
}timer;
extern uint8_t car_shoud_stop;

//����������������
void encoder_INST_IRQHandler(void)
{
	//20ms�����жϴ���
	if( DL_TimerA_getPendingInterrupt(encoder_INST) == DL_TIMER_IIDX_ZERO )
	{
        /*speed*/
        car_instance.l_speed = -motor_encoder.temp_count/736.0f*50.0f;
        car_instance.r_speed = ((int16_t )QEI_0_INST->COUNTERREGS.CTR)/1472.0f*50.0f;
        QEI_0_INST->COUNTERREGS.CTR=0;
      
#if USING_ENCODER_SUM ==1        
        /*ͳ��·��*/
        car_instance.encoder_sum-=motor_encoder.temp_count;
#endif
        motor_encoder.temp_count=0; 
        
        if(car_shoud_stop)
        {
            left_pid_struct.tar_val=0;
            right_pid_struct.tar_val=0;
            goto exit;
        }            
#if START_TRACKING==1   /*ѭ��*/     
        
        get_no_mcu_scale_data(&sensor_bits);//��ȡ�Ҷȴ���������
        
        if(sensor_bits.byte!=0xff)
        {
            float speed_offset=pid_control(&scale_pid_struct,scale_val(&sensor_bits)); 
            left_pid_struct.tar_val= car_instance.global_speed+ speed_offset;
            right_pid_struct.tar_val= car_instance.global_speed-speed_offset;
        }
        
#if  NO_STD_TURN ==1
        else
        {
            if(timer.is_start==0)
            {
                car_instance.cur_turn_point_num++;//�յ�������
                timer.is_start=1;//����һ����ʱ��
                /*����Ƿ���1ģʽ*/
                if(menu.fahui1_start)
                {
                     uart2_send("s\r\n",3);  //����c8t6ת
                }
            }
//            car_instance.global_speed=0.5;
            left_pid_struct.tar_val=   car_instance.global_speed - ADD_SPEED;
            right_pid_struct.tar_val=  car_instance.global_speed + ADD_SPEED;
        }
        if(timer.is_start==1)
        {
            if(++timer.cnt>CNT_LENGTH)
            {
//                car_instance.global_speed=1;
                timer.cnt=0;
                timer.is_start=0;
            }
        }
#endif 
        
#if FORCE_TURN ==1      
        else   /*������ʧǿ��ת��*/
        {            
            if(timer.is_start==0)
            {
                car_instance.cur_turn_point_num++;//�յ�������
                timer.is_start=1;//����һ����ʱ��
                /*ֱ��һ�ξ���*/
                left_pid_struct.tar_val=  car_instance.global_speed;
                right_pid_struct.tar_val= car_instance.global_speed;
            }
        }
        
        /*��ʱ���յ������*/
        if(timer.is_start==1)
        {
            ++timer.cnt;
            if(timer.cnt<STRAIGHT_AHEAD_TIME)
            {
                /*ֱ��*/
            }                   /*ʱ��С�ڸ���ʱ��*/            // ����  /*  */
            else if(timer.cnt<STRAIGHT_AHEAD_TIME+TURE_LEFT_TIME&&sensor_bits.byte==0xff)
            {
                /*ת��*/ 
                left_pid_struct.tar_val  = -car_instance.global_speed;
                right_pid_struct.tar_val =  car_instance.global_speed;
                /*����Ƿ���1ģʽ*/
                if(menu.fahui1_start)
                {
                     uart2_send("s\r\n",3);  //����c8t6ת
                }
            }
            else if(timer.cnt>CNT_LENGTH) 
            {
                /*��ʱ����*/
                //���Լ����һ�ιյ�
                timer.cnt=0;
                timer.is_start=0;
            }    
        } 
#endif   /*FORCE_TURN*/
#endif /*START_TRACKING*/

        /* �趨Ȧ���� */        
        if(menu.mode_0_start)
        {
            if(car_instance.cur_turn_point_num>=menu.turn_point_num)
            {
                /* ��ֹ��Ȧ */
                menu.mode_0_start=0;
                set_gloab_speed(0);        
            }
        }

        exit:
            
        /*ά��speed*/     
        l_out=pid_control(&left_pid_struct,car_instance.l_speed);  
        r_out=pid_control(&right_pid_struct,car_instance.r_speed);     
       
        /*��������ֵ*/
        car_instance.give_l_power(l_out);
        car_instance.give_r_power(r_out);        
	}
}

void set_gloab_speed(float speed)
{
     car_instance.global_speed=speed;
     car_instance.set_speed(speed,speed);    
}