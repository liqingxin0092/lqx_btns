#ifndef __HMI_H
#define __HMI_H

#include "ti_msp_dl_config.h"

typedef struct
{
    uint8_t mode_0_start:1;
    uint8_t fahui1_start:1;
    uint8_t mode;  //0:Éè¶¨È¦ÊıÅÜ 
    uint8_t turn_point_num;//mode0
    uint8_t num_of_turns;
}menu_t;
extern volatile menu_t menu;

typedef struct 
{
    uint8_t is_start:1;
    uint32_t cnt;
}timer_t;
extern timer_t car_delay_start_timer;

void change_mode(void);
void hmi_task(void);

#endif

