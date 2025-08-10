#ifndef __MOTOR_H
#define __MOTOR_H

#include "ti_msp_dl_config.h"
#include "pid_ctrl.h"

//1Ȧ:11300,2Ȧ:22600
#define NUM_OF_ENCODERS_PER_REVOLUTION 11300

typedef enum {
    FORWARD,  // ����
    REVERSAL  // ����
} ENCODER_DIR;

typedef struct {
    volatile int temp_count; //����ʵʱ����ֵ
    int count;         						 //���ݶ�ʱ��ʱ����µļ���ֵ
    ENCODER_DIR dir;            	 //��ת����
} ENCODER_RES;

typedef struct
{
    uint8_t mode:1; //0:������. 1:ѭ��
    float l_speed;//���
    float r_speed;//Ӳ��
    float global_speed;
    uint8_t cur_turn_point_num;
    volatile uint32_t encoder_sum;
    void (*give_l_power)(int16_t power);
    void (*give_r_power)(int16_t power);
    void (*set_speed)(float l_s,float r_s);
}car_t ;
  
extern car_t car_instance;
extern pid_t left_pid_struct;
extern pid_t right_pid_struct;

void encoder_init(void);
int get_encoder_count(void);
ENCODER_DIR get_encoder_dir(void);
void encoder_update(void);
void set_gloab_speed(float speed);

#endif 

