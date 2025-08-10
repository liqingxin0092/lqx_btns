//
// author : liqingxin0092
// date   : 25/6/5
//
#ifndef LQX_BTNS_CODE_H
#define LQX_BTNS_CODE_H

/*每个按键单元有几组事件*/
#define KEY_EVENT_PER_NUM 9

//这个根据情况设哈...

/*按键码*/
typedef enum {
    key0_pressed=0,
    key0_released,
    key0_long_pressed,
    key0_repeated,
    key0_double_combo,
    key0_treble_combo,
    key0_four_hit_combo,
    key0_five_hit_combo,
    key0_six_hit_combo,

    key1_pressed,
    key1_released,
    key1_long_pressed,
    key1_repeated,
    key1_double_combo,
    key1_treble_combo,
    key1_four_hit_combo,
    key1_five_hit_combo,
    key1_six_hit_combo,

    key2_pressed,
    key2_released,
    key2_long_pressed,
    key2_repeated,
    key2_double_combo,
    key2_treble_combo,
    key2_four_hit_combo,
    key2_five_hit_combo,
    key2_six_hit_combo,

    key3_pressed,
    key3_released,
    key3_long_pressed,
    key3_repeated,
    key3_double_combo,
    key3_treble_combo,
    key3_four_hit_combo,
    key3_five_hit_combo,
    key3_six_hit_combo,

    key4_pressed,
    key4_released,
    key4_long_pressed,
    key4_repeated,
    key4_double_combo,
    key4_treble_combo,
    key4_four_hit_combo,
    key4_five_hit_combo,
    key4_six_hit_combo,
    
    key5_pressed,
    key5_released,
    key5_long_pressed,
    key5_repeated,
    key5_double_combo,
    key5_treble_combo,
    key5_four_hit_combo,
    key5_five_hit_combo,
    key5_six_hit_combo,

    com_key0_pressed,
    com_key0_released,
    com_key0_long_pressed,
    com_key0_repeated,
    com_key0_double_combo,
    com_key0_treble_combo,
    com_key0_four_hit_combo,
    com_key0_five_hit_combo,
    com_key0_six_hit_combo,

    com_key1_pressed,
    com_key1_released,
    com_key1_long_pressed,
    com_key1_repeated,
    com_key1_double_combo,
    com_key1_treble_combo,
    com_key1_four_hit_combo,
    com_key1_five_hit_combo,
    com_key1_six_hit_combo,
}key_code_e;

#endif //LQX_BTNS_CODE_H