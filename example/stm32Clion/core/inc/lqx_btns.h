//
// author : liqingxin0092
// date   : 25/6/5
//
// 作者旨在写一个按键驱动,每个按键都可以自由配置.
// 支持单击,双击,三击,四击....可配置, 长摁,重复,组合按键,总之就是每个按键单元高度可配置化...
//
// 什么是按键单元?
// 单个按键或者组合按键. 比如:key1,key2,key1+key2.
//
// 有局限性吧,这个key初始化函数,想支持全平台,又不想写每种芯片,就用个函数指针,用户自己实现吧.

#ifndef LQX_BTNS_H
#define LQX_BTNS_H
#include <stdint.h>

#include "lqx_btns_code.h"

typedef uint16_t e_t ;

void key_init(void);                          //按键的初始化函数
void key_detect(void);                        //10ms运行一次
uint8_t get_key_event(e_t* key_code) ;        //获取按键事件

#endif //LQX_BTNS_H
