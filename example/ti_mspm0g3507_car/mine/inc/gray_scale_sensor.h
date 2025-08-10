#ifndef __GRAY_SCALE_SENSOR_H
#define __GRAY_SCALE_SENSOR_H

#include "ti_msp_dl_config.h"
#include "pid_ctrl.h"

typedef union
{
    struct
    {
        uint8_t way0:1;
        uint8_t way1:1;
        uint8_t way2:1;
        uint8_t way3:1;
        uint8_t way4:1;
        uint8_t way5:1;
        uint8_t way6:1;
        uint8_t way7:1;
    }bits;
    uint8_t byte;
}sensor_data_t;

extern volatile sensor_data_t sensor_bits;
extern pid_t scale_pid_struct;

void read_gray_scale_sensor(volatile sensor_data_t* instance);
int8_t scale_val(const sensor_data_t* data);
bool scale_is_trigger(void);

/*ÎÞmcu*/
void get_no_mcu_scale_data(sensor_data_t* data);
    
#endif

