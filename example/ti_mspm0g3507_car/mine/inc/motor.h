#ifndef __MOTOR_H
#define __MOTOR_H

#include "ti_msp_dl_config.h"
#include "pid_ctrl.h"

//1圈:11300,2圈:22600
#define NUM_OF_ENCODERS_PER_REVOLUTION 11300

typedef enum {
    FORWARD,  // 正向
    REVERSAL  // 反向
} ENCODER_DIR;

typedef struct {
    volatile int temp_count; //保存实时计数值
    int count;         						 //根据定时器时间更新的计数值
    ENCODER_DIR dir;            	 //旋转方向
} ENCODER_RES;

typedef struct
{
    uint8_t mode:1; //0:陀螺仪. 1:循迹
    float l_speed;//软件
    float r_speed;//硬件
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

