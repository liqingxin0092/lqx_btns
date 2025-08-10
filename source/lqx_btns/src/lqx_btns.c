//
// author : liqingxin0092
// date   : 25/6/5
//
#include "lqx_btns.h"

#include <strings.h>

#include "lqx_btns_port.h"

/*按键circle buffer*/
typedef struct {
    e_t *buffer;
    uint16_t length;
    uint16_t w_ptr;
    uint16_t r_ptr;
}circle_buffer_t;

/*按键缓冲区*/
static e_t key_buffer[BUFFER_MAX_LENGTH];

/*按键环形缓冲区结构体*/
static circle_buffer_t key_circle_buffer;

/*环形缓冲区增加一个值*/
static void add_num(uint16_t* num,uint16_t max)
{
    (*num)++;
    if (*num==max)
        *num=0;
}

/*写环形缓冲区*/
static int8_t write_circle_buffer(circle_buffer_t *circle_buffer, e_t val)
{
    uint16_t temp=circle_buffer->w_ptr;
    add_num(&temp,BUFFER_MAX_LENGTH);
    if (temp==circle_buffer->r_ptr) {
        return 1;//满了
    }
    circle_buffer->buffer[circle_buffer->w_ptr]=val;
    add_num(&circle_buffer->w_ptr,BUFFER_MAX_LENGTH);
    return 0;
}

/*读环形缓冲区*/
//返回值:1,满了. 0正常
static int8_t read_circle_buffer(circle_buffer_t *circle_buffer, e_t *val)
{
    if (circle_buffer->r_ptr==circle_buffer->w_ptr)
    {
        return 1; //满了
    }
    *val=circle_buffer->buffer[circle_buffer->r_ptr];
    add_num(&circle_buffer->r_ptr,BUFFER_MAX_LENGTH);
    return 0;
}

/*按键单元是否工作*/
//1,按键单元起作用. 0,按键单元没有起作用
static uint8_t is_KeyUnit_func(uint8_t index)
{
    key_unit_t* key_unit=key_get_unit();
    /*检测单键*/
    if (index<KEY_ACTUAL_NUM)
    {
        uint8_t cnt=0;        //触发的数量
        uint8_t act_key=0xff; //触发的按键序号

        /*检测同期其他按键有没有被按下*/
        //因为判定单键只允许一个按键按下
        for (uint8_t i=0;i<KEY_ACTUAL_NUM;i++) {
            if (is_key_pressed(&key_unit[i])) {
                cnt++;
                act_key=i;
            }
        }

        /*如果只有一个按键被按下并且被按下的那个键正是要检测的序号*/
        if (cnt==1&&act_key==index) {
            return 1;
        }
        return 0;
    }

    /*检测组合键*/
    if(key_get_func()[index-KEY_ACTUAL_NUM]()) {
            return 1;
    }
    return 0;
}

/*启动计数*/
//time:单位1ms
static void key_start_counting(key_unit_t *unit_exam,uint16_t times) {
    unit_exam->timer_load=times;
    unit_exam->timer_cnt=0;
    unit_exam->flags.timer_state=1;
}

/*结束计数*/
static void key_stop_counting(key_unit_t *unit_exam) {
    unit_exam->timer_cnt=0;
    unit_exam->flags.timer_state=0;
}

/*维持一个key_timer*/
static void run_one_key_timer(uint8_t index) {
    /*获取当前按键单元*/
    key_unit_t *cur_unit=&key_get_unit()[index];
    if (cur_unit->flags.timer_state==1) {
        cur_unit->timer_cnt++;
        if (cur_unit->timer_cnt==cur_unit->timer_load) {
            cur_unit->flags.timer_state=0; //到时/未启用
            cur_unit->timer_cnt=0;         //计数值清零
            cur_unit->combo_cnt=0;         //连击数清零
        }
    }
}

/*发送连击事件*/
static void send_combo_event(key_unit_t * cur_unit,uint8_t index) {
    /*连击判定结束*/
    if (cur_unit->flags.timer_state==1) //如果上一次计数未到时
    {
        cur_unit->combo_cnt++;
        key_stop_counting(cur_unit);//停止计数
    }
    if (cur_unit->combo_cnt) {
        write_circle_buffer(&key_circle_buffer, KEY_EVENT_PER_NUM*index+cur_unit->combo_cnt+3);//连击
        if (cur_unit->combo_cnt+1==MAX_HIT) { //如果达到最大限度,连击结束
            cur_unit->combo_cnt=0;
        }
    }
    else {
        /*摁下事件*/
        write_circle_buffer(&key_circle_buffer, KEY_EVENT_PER_NUM*index);//按下
    }
}

/*判断按键单元的触发状态并且入队*/
static void detect_one_key_unit(uint8_t index) {

    /*获取当前按键单元*/
    key_unit_t *cur_unit=&key_get_unit()[index];

    /*如果按键按下了*/
    if (is_KeyUnit_func(index)) { //检测到按键按下

        /*滤波*/
        if (cur_unit->filter_cnt<KEY_FILTERING_TIME) {
            cur_unit->filter_cnt++;
            return;
        }

        if (cur_unit->flags.state==0)
        {
            cur_unit->flags.state=1;//按键按下了
            send_combo_event(cur_unit,index);//发送连击or按下事件
        }

        if (cur_unit->flags.state==1) {
            /*长摁检测*/
            if (cur_unit->long_time_counter<LONG_MAX_TIME) {
                cur_unit->long_time_counter++;
            }
            else {
                if (cur_unit->flags.long_time_state==0) {
                    cur_unit->flags.long_time_state=1;
                    write_circle_buffer(&key_circle_buffer, KEY_EVENT_PER_NUM*index+2);//长摁事件
                }
            }

            /*重复事件*/      //首先得启用use_repeat
            if (cur_unit->flags.use_repeat==1) {
                if(cur_unit->flags.long_time_state==1) {
                    if (cur_unit->repeat_cnt<REPEAT_TIME) {
                        cur_unit->repeat_cnt++;
                    }
                    else {
                        cur_unit->repeat_cnt=0;
                        write_circle_buffer(&key_circle_buffer, KEY_EVENT_PER_NUM*index+3);//重复事件
                    }
                }
            }
        }
    }
    else { //检测到按键未按下

        /*滤波*/
        if (cur_unit->filter_cnt>0) {
            cur_unit->filter_cnt--;
            return;
        }

        if (cur_unit->flags.state==1)
        {
            /*启动连发计数*/
            key_start_counting(cur_unit,COMBO_TOLERANCE_TIME);

            cur_unit->flags.state=0;//按键松开

#if STOP_SEND_RELEASE == 1
            if (cur_unit->combo_cnt==0)
#endif

                /*松开事件*/
                write_circle_buffer(&key_circle_buffer, KEY_EVENT_PER_NUM*index+1);//松开
        }

        /*清除状态*/
        cur_unit->flags.long_time_state=0; //肯定结束长摁了
        cur_unit->long_time_counter=0;
        cur_unit->repeat_cnt=0;
    }
}

/*按键初始化*/
void key_init(void)
{
    uint8_t i;

    /*硬件初始化*/
    for (i=0;i<KEY_ACTUAL_NUM;i++)
    {
        key_hw_init(&key_get_unit()[i].hw_info); //按键初始化
    }

    /*缓冲区初始化*/
    key_circle_buffer.buffer = key_buffer;
    key_circle_buffer.length = BUFFER_MAX_LENGTH;
}

/*按键检测*/
//这个函数每10ms被调用一次
void key_detect(void)
{
    uint16_t key_unit_num=key_get_unit_num();
    for (uint16_t i=0;i<key_unit_num;i++) {
        detect_one_key_unit(i);     //检测按键事件
        run_one_key_timer(i);       //检测按键timer
    }
}

/*获取最近按键事件*/
//返回值:1,存在按键事件. 0,不存在按键事件
uint8_t get_key_event(e_t* key_code) {
    if (!read_circle_buffer(&key_circle_buffer, key_code)) {
        return 1;
    }
    return 0;
}
