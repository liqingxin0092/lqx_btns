#ifndef __HWT101_300_H
#define __HWT101_300_H

#include "ti_msp_dl_config.h"

/*这里用的是串口2*/
#define HWT_UART UART_1_INST
#define HWT_UART_HANDLE UART_1_INST_IRQHandler

typedef struct
{
    volatile uint8_t hwt_content_type;
}hwt_flags_t;

float hwt_get_yaw(void);

#endif 


