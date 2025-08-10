#ifndef __JY61P_H
#define __JY61P_H

#include <stdint.h>
#include "pid_ctrl.h"
typedef struct
{
    volatile uint8_t jy61p_content_type;
}jy61p_flags_t;

float get_yaw(void);
extern pid_t jy61p_pid_struct ;

#endif 
