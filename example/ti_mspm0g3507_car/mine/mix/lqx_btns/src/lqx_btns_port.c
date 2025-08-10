//
// author : liqingxin0092
// date   : 25/6/5
//
#include "lqx_btns_port.h"

/*用户实现按键的配置*/
// 这里比如有5个按键,2个组合键:key0+key1,key2+key3
static key_unit_t key_unit[] = {
    /*key1*/
    {
        .hw_info=
        {
            .GPIOx=KEY_key1_PORT,
            .GPIO_Pin= KEY_key1_PIN,
        },
        .active_state = 0,          //触发边沿设置. 1,上升沿. 0,下降沿
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },

    /*key2*/
    {
        .hw_info=
        {
            .GPIOx=KEY_key2_PORT,
            .GPIO_Pin= KEY_key2_PIN,
        },
        .active_state = 0,          //触发边沿设置. 1,上升沿. 0,下降沿
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },
    /*key3*/
    {
        .hw_info=
        {
            .GPIOx=KEY_key3_PORT,
            .GPIO_Pin= KEY_key3_PIN,
        },
        .active_state = 0,          //触发边沿设置. 1,上升沿. 0,下降沿
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },
    /*key4*/
//    {
//        .hw_info=
//        {
//            .GPIOx=KEY_key4_PORT,
//            .GPIO_Pin= KEY_key4_PIN,
//        },
//        .active_state = 0,          //触发边沿设置. 1,上升沿. 0,下降沿
//        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
//    },
    /*key5*/
    {
        .hw_info=
        {
            .GPIOx=KEY_key5_PORT,
            .GPIO_Pin= KEY_key5_PIN,
        },
        .active_state = 0,          //触发边沿设置. 1,上升沿. 0,下降沿
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },
    /*key6*/
    {
        .hw_info=
        {
            .GPIOx=KEY_key6_PORT,
            .GPIO_Pin= KEY_key6_PIN,
        },
        .active_state = 0,          //触发边沿设置. 1,上升沿. 0,下降沿
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },
    /*以下是用户定义的按键组合*/
    /*key0&&key1*/
    {
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },
    /*key2&&key3*/
    {
        .flags.use_repeat = 1,      //支持连发情况设置.1,支持. 0,不支持
    },
};

#if COMBINE_KEY_NUM!=0

/*用户组合键检测函数的声明*/
static uint8_t test0(void);
static uint8_t test1(void);

/*用户登记组合键检测函数*/
static jgmt_t is_combination_trigger[]={
    test0,test1,
};

#endif

/************************ 用户需要实现以下函数 ********************************************/
// 注意实现下列函数不要改名字

/*用户实现单个按键检测函数*/
//1,触发了.     0,没触发
uint8_t is_key_pressed(key_unit_t* key_unit_temp)
{
    if((bool)(key_unit_temp->hw_info.GPIOx->DIN31_0 & key_unit_temp->hw_info.GPIO_Pin) == key_unit_temp->active_state)
        return 1;
    return 0;
}

/*用户实现组合按键的检测函数*/
//如果没有组合键可以不用实现
//名字可以自定义
//实现完了记得到前面去注册
//可以使用上面那个函数
//1,触发了. 0,没触发

//eg:
/*按键0和按键1组合*/
static uint8_t test0(void) {
    if (is_key_pressed(&key_unit[0])&&is_key_pressed(&key_unit[1])) {
        return 1;
    }
    return 0;
}

/*按键2和按键3组合*/
static uint8_t test1(void) {
    if (is_key_pressed(&key_unit[2])&&is_key_pressed(&key_unit[3])) {
        return 1;
    }
    return 0;
}


/************************ 用户需要实现以上函数 ********************************************/

/*帮助核心文件获取到用户的配置*/
key_unit_t *key_get_unit(void)
{
    return key_unit;
}

/*帮助核心文件获取到用户的按键单元数*/
uint8_t key_get_unit_num(void)
{
    return (uint8_t)sizeof(key_unit) / sizeof(key_unit_t);
}

/*获取函数*/
jgmt_t *key_get_func(void) {
    return is_combination_trigger;
}