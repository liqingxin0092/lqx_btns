//
// author : liqingxin0092
// date   : 25/6/5
//
#ifndef LQX_BTNS_PORT_H
#define LQX_BTNS_PORT_H

#include "stm32f10x_conf.h"

/************************   以下是用户配置区   ********************************************/

/*按键基本信息结构体*/
//为单个按键初始化,检测函数提供一个自定义结构体
//用户自行填写结构体里的内容,不要改结构体名字
typedef struct
{
    /*比如stm32*/
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint8_t Mode;
}hw_info_t;

/*滤波时长*/
//单位:10ms
#define KEY_FILTERING_TIME 3

/*组合键的数量*/
#define COMBINE_KEY_NUM 2

/*长摁检测时间*/
//单位:10ms,
#define LONG_MAX_TIME 100

/*重复事件间隔*/
//单位:10ms
#define REPEAT_TIME 30

/*连击容忍时间*/
//单位:10ms
#define COMBO_TOLERANCE_TIME 40

/*最多支持多少连击*/
//注意改一下lqx_btns_code.h
#define MAX_HIT 6

/*按键缓冲区的大小*/
//单位:Byte
//注意判断队满会占用一个空间,实际容量是: 这个宏 - 1
#define BUFFER_MAX_LENGTH 5

/*连击时停止发送松开事件*/
#define STOP_SEND_RELEASE 1

/************************   以上是用户配置区   ********************************************/

/*按键单元结构体*/
typedef struct
{
    hw_info_t hw_info;     //这个在port中实现
    uint8_t active_state;  //1,上升沿触发. 0,下降沿触发
    uint8_t filter_cnt;    //滤波计数器
    uint8_t long_time_counter; //长摁计数
    uint8_t repeat_cnt;    //重复计数器
    uint16_t timer_cnt;
    uint16_t timer_load;
    uint8_t combo_cnt;
    volatile struct {
        uint8_t state:1;
        uint8_t long_time_state:1;
        uint8_t use_repeat:1;
        uint8_t timer_state:1;
    }flags;
}key_unit_t;

/*组合键判断的函数*/
typedef  uint8_t (*jgmt_t)(void);

/*机械按键的数量*/
#define KEY_ACTUAL_NUM (key_get_unit_num()-COMBINE_KEY_NUM)

/*核心文件实现需要以下接口*/
void key_hw_init(hw_info_t *info);              //用户实现的初始化函数
uint8_t is_key_pressed(key_unit_t* key_unit);   //用户实现单个按键检测函数
key_unit_t *key_get_unit(void);                 //获取用户的配置
uint8_t key_get_unit_num(void);                 //获取到用户的按键单元数
jgmt_t *key_get_func(void);                     //获取用户的组合键函数

#endif //LQX_BTNS_PORT_H
