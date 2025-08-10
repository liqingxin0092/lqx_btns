/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM_l */
#define PWM_l_INST                                                         TIMG0
#define PWM_l_INST_IRQHandler                                   TIMG0_IRQHandler
#define PWM_l_INST_INT_IRQN                                     (TIMG0_INT_IRQn)
#define PWM_l_INST_CLK_FREQ                                             20000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_l_C0_PORT                                                 GPIOA
#define GPIO_PWM_l_C0_PIN                                         DL_GPIO_PIN_12
#define GPIO_PWM_l_C0_IOMUX                                      (IOMUX_PINCM34)
#define GPIO_PWM_l_C0_IOMUX_FUNC                     IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_PWM_l_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_l_C1_PORT                                                 GPIOA
#define GPIO_PWM_l_C1_PIN                                         DL_GPIO_PIN_13
#define GPIO_PWM_l_C1_IOMUX                                      (IOMUX_PINCM35)
#define GPIO_PWM_l_C1_IOMUX_FUNC                     IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_l_C1_IDX                                    DL_TIMER_CC_1_INDEX




/* Defines for QEI_0 */
#define QEI_0_INST                                                         TIMG8
#define QEI_0_INST_IRQHandler                                   TIMG8_IRQHandler
#define QEI_0_INST_INT_IRQN                                     (TIMG8_INT_IRQn)
/* Pin configuration defines for QEI_0 PHA Pin */
#define GPIO_QEI_0_PHA_PORT                                                GPIOA
#define GPIO_QEI_0_PHA_PIN                                         DL_GPIO_PIN_1
#define GPIO_QEI_0_PHA_IOMUX                                      (IOMUX_PINCM2)
#define GPIO_QEI_0_PHA_IOMUX_FUNC                     IOMUX_PINCM2_PF_TIMG8_CCP0
/* Pin configuration defines for QEI_0 PHB Pin */
#define GPIO_QEI_0_PHB_PORT                                                GPIOA
#define GPIO_QEI_0_PHB_PIN                                         DL_GPIO_PIN_0
#define GPIO_QEI_0_PHB_IOMUX                                      (IOMUX_PINCM1)
#define GPIO_QEI_0_PHB_IOMUX_FUNC                     IOMUX_PINCM1_PF_TIMG8_CCP1


/* Defines for encoder */
#define encoder_INST                                                     (TIMA0)
#define encoder_INST_IRQHandler                                 TIMA0_IRQHandler
#define encoder_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define encoder_INST_LOAD_VALUE                                         (19999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_40_MHZ_9600_BAUD                                       (260)
#define UART_0_FBRD_40_MHZ_9600_BAUD                                        (27)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART3
#define UART_1_INST_FREQUENCY                                           80000000
#define UART_1_INST_IRQHandler                                  UART3_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                        DL_GPIO_PIN_25
#define GPIO_UART_1_TX_PIN                                        DL_GPIO_PIN_26
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM55)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM59)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM55_PF_UART3_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM59_PF_UART3_TX
#define UART_1_BAUD_RATE                                                  (9600)
#define UART_1_IBRD_80_MHZ_9600_BAUD                                       (520)
#define UART_1_FBRD_80_MHZ_9600_BAUD                                        (53)
/* Defines for UART_2 */
#define UART_2_INST                                                        UART1
#define UART_2_INST_FREQUENCY                                           40000000
#define UART_2_INST_IRQHandler                                  UART1_IRQHandler
#define UART_2_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_2_RX_PORT                                                GPIOA
#define GPIO_UART_2_TX_PORT                                                GPIOA
#define GPIO_UART_2_RX_PIN                                        DL_GPIO_PIN_18
#define GPIO_UART_2_TX_PIN                                        DL_GPIO_PIN_17
#define GPIO_UART_2_IOMUX_RX                                     (IOMUX_PINCM40)
#define GPIO_UART_2_IOMUX_TX                                     (IOMUX_PINCM39)
#define GPIO_UART_2_IOMUX_RX_FUNC                      IOMUX_PINCM40_PF_UART1_RX
#define GPIO_UART_2_IOMUX_TX_FUNC                      IOMUX_PINCM39_PF_UART1_TX
#define UART_2_BAUD_RATE                                                (115200)
#define UART_2_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_2_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_3 */
#define UART_3_INST                                                        UART2
#define UART_3_INST_FREQUENCY                                           40000000
#define UART_3_INST_IRQHandler                                  UART2_IRQHandler
#define UART_3_INST_INT_IRQN                                      UART2_INT_IRQn
#define GPIO_UART_3_RX_PORT                                                GPIOA
#define GPIO_UART_3_TX_PORT                                                GPIOA
#define GPIO_UART_3_RX_PIN                                        DL_GPIO_PIN_24
#define GPIO_UART_3_TX_PIN                                        DL_GPIO_PIN_23
#define GPIO_UART_3_IOMUX_RX                                     (IOMUX_PINCM54)
#define GPIO_UART_3_IOMUX_TX                                     (IOMUX_PINCM53)
#define GPIO_UART_3_IOMUX_RX_FUNC                      IOMUX_PINCM54_PF_UART2_RX
#define GPIO_UART_3_IOMUX_TX_FUNC                      IOMUX_PINCM53_PF_UART2_TX
#define UART_3_BAUD_RATE                                                  (9600)
#define UART_3_IBRD_40_MHZ_9600_BAUD                                       (260)
#define UART_3_FBRD_40_MHZ_9600_BAUD                                        (27)





/* Defines for ADC12_0 */
#define ADC12_0_INST                                                        ADC0
#define ADC12_0_INST_IRQHandler                                  ADC0_IRQHandler
#define ADC12_0_INST_INT_IRQN                                    (ADC0_INT_IRQn)
#define ADC12_0_ADCMEM_0                                      DL_ADC12_MEM_IDX_0
#define ADC12_0_ADCMEM_0_REF                     DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC12_0_ADCMEM_0_REF_VOLTAGE_V                                       3.3
#define GPIO_ADC12_0_C0_PORT                                               GPIOA
#define GPIO_ADC12_0_C0_PIN                                       DL_GPIO_PIN_27



/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOB)

/* Defines for PIN_0: GPIOB.22 with pinCMx 50 on package pin 21 */
#define GPIO_GRP_0_PIN_0_PIN                                    (DL_GPIO_PIN_22)
#define GPIO_GRP_0_PIN_0_IOMUX                                   (IOMUX_PINCM50)
/* Defines for PIN_21: GPIOB.21 with pinCMx 49 on package pin 20 */
#define KEY_PIN_21_PORT                                                  (GPIOB)
#define KEY_PIN_21_PIN                                          (DL_GPIO_PIN_21)
#define KEY_PIN_21_IOMUX                                         (IOMUX_PINCM49)
/* Defines for key1: GPIOB.15 with pinCMx 32 on package pin 3 */
#define KEY_key1_PORT                                                    (GPIOB)
#define KEY_key1_PIN                                            (DL_GPIO_PIN_15)
#define KEY_key1_IOMUX                                           (IOMUX_PINCM32)
/* Defines for key2: GPIOB.13 with pinCMx 30 on package pin 1 */
#define KEY_key2_PORT                                                    (GPIOB)
#define KEY_key2_PIN                                            (DL_GPIO_PIN_13)
#define KEY_key2_IOMUX                                           (IOMUX_PINCM30)
/* Defines for key3: GPIOB.19 with pinCMx 45 on package pin 16 */
#define KEY_key3_PORT                                                    (GPIOB)
#define KEY_key3_PIN                                            (DL_GPIO_PIN_19)
#define KEY_key3_IOMUX                                           (IOMUX_PINCM45)
/* Defines for key4: GPIOA.21 with pinCMx 46 on package pin 17 */
#define KEY_key4_PORT                                                    (GPIOA)
#define KEY_key4_PIN                                            (DL_GPIO_PIN_21)
#define KEY_key4_IOMUX                                           (IOMUX_PINCM46)
/* Defines for key5: GPIOA.22 with pinCMx 47 on package pin 18 */
#define KEY_key5_PORT                                                    (GPIOA)
#define KEY_key5_PIN                                            (DL_GPIO_PIN_22)
#define KEY_key5_IOMUX                                           (IOMUX_PINCM47)
/* Defines for key6: GPIOB.20 with pinCMx 48 on package pin 19 */
#define KEY_key6_PORT                                                    (GPIOB)
#define KEY_key6_PIN                                            (DL_GPIO_PIN_20)
#define KEY_key6_IOMUX                                           (IOMUX_PINCM48)
/* Port definition for Pin Group LCD */
#define LCD_PORT                                                         (GPIOB)

/* Defines for SCL: GPIOB.9 with pinCMx 26 on package pin 61 */
#define LCD_SCL_PIN                                              (DL_GPIO_PIN_9)
#define LCD_SCL_IOMUX                                            (IOMUX_PINCM26)
/* Defines for SDA: GPIOB.8 with pinCMx 25 on package pin 60 */
#define LCD_SDA_PIN                                              (DL_GPIO_PIN_8)
#define LCD_SDA_IOMUX                                            (IOMUX_PINCM25)
/* Defines for RES: GPIOB.10 with pinCMx 27 on package pin 62 */
#define LCD_RES_PIN                                             (DL_GPIO_PIN_10)
#define LCD_RES_IOMUX                                            (IOMUX_PINCM27)
/* Defines for DC: GPIOB.11 with pinCMx 28 on package pin 63 */
#define LCD_DC_PIN                                              (DL_GPIO_PIN_11)
#define LCD_DC_IOMUX                                             (IOMUX_PINCM28)
/* Defines for CS: GPIOB.14 with pinCMx 31 on package pin 2 */
#define LCD_CS_PIN                                              (DL_GPIO_PIN_14)
#define LCD_CS_IOMUX                                             (IOMUX_PINCM31)
/* Defines for BLK: GPIOB.26 with pinCMx 57 on package pin 28 */
#define LCD_BLK_PIN                                             (DL_GPIO_PIN_26)
#define LCD_BLK_IOMUX                                            (IOMUX_PINCM57)
/* Port definition for Pin Group soft_encoder */
#define soft_encoder_PORT                                                (GPIOB)

/* Defines for PIN_A: GPIOB.0 with pinCMx 12 on package pin 47 */
// pins affected by this interrupt request:["PIN_A","PIN_B"]
#define soft_encoder_INT_IRQN                                   (GPIOB_INT_IRQn)
#define soft_encoder_INT_IIDX                   (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define soft_encoder_PIN_A_IIDX                              (DL_GPIO_IIDX_DIO0)
#define soft_encoder_PIN_A_PIN                                   (DL_GPIO_PIN_0)
#define soft_encoder_PIN_A_IOMUX                                 (IOMUX_PINCM12)
/* Defines for PIN_B: GPIOB.1 with pinCMx 13 on package pin 48 */
#define soft_encoder_PIN_B_IIDX                              (DL_GPIO_IIDX_DIO1)
#define soft_encoder_PIN_B_PIN                                   (DL_GPIO_PIN_1)
#define soft_encoder_PIN_B_IOMUX                                 (IOMUX_PINCM13)
/* Defines for l_out_0: GPIOB.16 with pinCMx 33 on package pin 4 */
#define car_l_out_0_PORT                                                 (GPIOB)
#define car_l_out_0_PIN                                         (DL_GPIO_PIN_16)
#define car_l_out_0_IOMUX                                        (IOMUX_PINCM33)
/* Defines for l_out_1: GPIOA.14 with pinCMx 36 on package pin 7 */
#define car_l_out_1_PORT                                                 (GPIOA)
#define car_l_out_1_PIN                                         (DL_GPIO_PIN_14)
#define car_l_out_1_IOMUX                                        (IOMUX_PINCM36)
/* Defines for r_out_0: GPIOA.15 with pinCMx 37 on package pin 8 */
#define car_r_out_0_PORT                                                 (GPIOA)
#define car_r_out_0_PIN                                         (DL_GPIO_PIN_15)
#define car_r_out_0_IOMUX                                        (IOMUX_PINCM37)
/* Defines for r_out_1: GPIOA.16 with pinCMx 38 on package pin 9 */
#define car_r_out_1_PORT                                                 (GPIOA)
#define car_r_out_1_PIN                                         (DL_GPIO_PIN_16)
#define car_r_out_1_IOMUX                                        (IOMUX_PINCM38)
/* Port definition for Pin Group scale */
#define scale_PORT                                                       (GPIOB)

/* Defines for en: GPIOB.17 with pinCMx 43 on package pin 14 */
#define scale_en_PIN                                            (DL_GPIO_PIN_17)
#define scale_en_IOMUX                                           (IOMUX_PINCM43)
/* Defines for ad2: GPIOB.18 with pinCMx 44 on package pin 15 */
#define scale_ad2_PIN                                           (DL_GPIO_PIN_18)
#define scale_ad2_IOMUX                                          (IOMUX_PINCM44)
/* Defines for ad1: GPIOB.23 with pinCMx 51 on package pin 22 */
#define scale_ad1_PIN                                           (DL_GPIO_PIN_23)
#define scale_ad1_IOMUX                                          (IOMUX_PINCM51)
/* Defines for ad0: GPIOB.24 with pinCMx 52 on package pin 23 */
#define scale_ad0_PIN                                           (DL_GPIO_PIN_24)
#define scale_ad0_IOMUX                                          (IOMUX_PINCM52)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_l_init(void);
void SYSCFG_DL_QEI_0_init(void);
void SYSCFG_DL_encoder_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_UART_2_init(void);
void SYSCFG_DL_UART_3_init(void);
void SYSCFG_DL_ADC12_0_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
