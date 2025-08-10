/**
*******************************************************************************
* @file        pid_ctrl.h
* @author      liqingxin0092
* @version     V1.0
* @date        2025-4-7
* @brief       pid����
* ���¼�¼:    ��
* ��pid,Ŀǰ�������Ƶ���,����������˵
*****************************************************************************************
*/

/*����ģ��*/
///*�����pid�ṹ��*/
////ת��:0~100   ---->  CCR:0~100
// pid_t motor_pid_struct=
//{
//   /* kp */
//     .kp= 1.09,
//   /* ki */
//     .ki=0.0014,
//   /* kd */
//     .kd=0.05,
//   /* tar_val */
//     .tar_val= 0,
//   /* ��������ۼ�ֵ����*/
//     .inc_max= 0,
//   /* ��������ۼ�ֵ����*/
//     .inc_min= 0,
//   /* ���ּ������ֵ*/
//     .inc_use_threshold= 40,
//   /* ���ֵ���� */
//     .out_max=40,
//   /* ���ֵ���� */
//     .out_min= 0,
//   /* ΢�����ȥȨ�� */
//     .dpart=0,
//   /* ���ƫ�� */
//     .out_offset= 0,
//   /* �������(�������) */
//     .out_threshold= 0,
// };

#ifndef __PID_CTRL_H
#define __PID_CTRL_H

#include "stdint.h"

/*pid�������ṹ��*/
// һ��Ҫ�����ø��Եı���
typedef struct
{
    float kp;
    float ki;
    float kd;
    float tar_val;

    /*�����޷�*/
    float inc_max;
    float inc_min;

    /*���ּ������ֵ*/
    float inc_use_threshold;

    /*����޷�*/
    float out_max;
    float out_min;

    float dpart; // ��ȥ��Ȩ��

    /*���ƫ��*/
    float out_offset;

    /*�������*/
    float out_threshold;

    /*�м����*/
    float err;
    float err_sum;

    /*΢�������м����*/
    float act;
    float act_last;

    /*΢���˲��м����*/
    float dout;
    
    /*����*/
    void *data;
    
} pid_t;

float pid_control(pid_t *pid_struct, float act_val);
float pid_control_gunzi(pid_t *pid_struct, float act_val, short gyro);

#endif
