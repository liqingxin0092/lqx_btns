/**
 *******************************************************************************
 * @file        pid_ctrl.c
 * @author      liqingxin0092
 * @version     V1.0
 * @date        2025-4-7
 * @brief       pid控制
 * 更新记录:    无
 * 这pid,目前是最完善的了,对于作者来说
 *****************************************************************************************
 */

#include "pid_ctrl.h"
#include "math.h"

/*pid 原理*/
// 比例项提供一个基础,可以加速系统响应.
// 只有比例项,当误差为0时,比例项输出为0,不起作用了.
// 当自然界往下掉的那个力(摩擦力,温度扩散)和当前误差对应的驱动力相等时,就形成了稳态误差.

// 积分项的一个缺点是系统的滞后性,如果目标值前后相差很大,要花点时间调整误差的和.

// 微分项,如果误差在不断减小,这次误差减去上次误差是一个负值,因此微分项是一个阻力.
// 微分项总会和pi对着干,前后误差相差越大,阻力越强.

// 变速积分和积分分离是并列的

// 微分先行:删除了误差值突变时的很大的正向力.
// 微分输出滤波:微分项容易收到噪声的影响.

/*pid控制器*/
// 位置式pid
// 积分限幅,输出限幅,积分分离,微分滤波功能不用给0
// act_val:实际值
// 返回值:输出给发力方的数据
float pid_control(pid_t *pid_struct, float act_val)
{
    float out_val = 0;
    float tar_temp = 0;
    
    if((int)pid_struct->data==1)
    {
        if(pid_struct->tar_val < 0)
        {
             if(act_val>0)
             {
                 float temp1 =  180+ pid_struct->tar_val;
                 float temp2 =  180-act_val;    
                 if (temp1+temp2<180)
                    pid_struct->err = temp1+temp2;
                 else
                    pid_struct->err =temp1+temp2-360;                     
             }
             else 
                 pid_struct->err = pid_struct->tar_val - act_val;
        }
        else if(pid_struct->tar_val >= 0)
        {
             if(act_val<0)
             {
                 float temp1 =  180- pid_struct->tar_val;
                 float temp2 =  180+act_val;                 
                 if (temp1+temp2<180)
                    pid_struct->err = -temp1-temp2;
                 else
                    pid_struct->err =360-temp1-temp2;
             }
             else 
                 pid_struct->err = pid_struct->tar_val - act_val;
        }
    }
    else
    {
        pid_struct->err = pid_struct->tar_val - act_val;
    }

    /*记录上一次实际值和这次实际值*/
    pid_struct->act_last = pid_struct->act;
    pid_struct->act = act_val;

    /*输出死区*/
    if (fabs(pid_struct->err) < pid_struct->out_threshold)
    {

        out_val = 0;
        goto exit;
    }

    /*误差累积*/
    if (pid_struct->ki != 0) // ki有数的情况下才累计误差
    {
        pid_struct->err_sum += pid_struct->err;
    }
    else // ki没数不累计误差,防止猛增
    {
        pid_struct->err_sum = 0;
    }

    /*积分限幅*/
    if (pid_struct->inc_max != 0)
    {
        if (pid_struct->err_sum > pid_struct->inc_max)
            pid_struct->err_sum = pid_struct->inc_max;
    }
    if (pid_struct->inc_min != 0)
    {
        if (pid_struct->err_sum < pid_struct->inc_min)
            pid_struct->err_sum = pid_struct->inc_min;
    }

    /*微分输出滤波*/
    pid_struct->dout = (1 - pid_struct->dpart) * -pid_struct->kd * (pid_struct->act - pid_struct->act_last) + pid_struct->dpart * pid_struct->dout;

    /*输出值计算*/
    out_val = pid_struct->kp * pid_struct->err + pid_struct->ki * pid_struct->err_sum + pid_struct->dout;

    /*输出偏移*/
    if (pid_struct->out_offset > 0)
        out_val += pid_struct->out_offset;
    else if (pid_struct->out_offset < 0)
        out_val -= pid_struct->out_offset;

    /*输出限幅*/
    if (pid_struct->out_max != 0)
    {
        if (out_val > pid_struct->out_max)
            out_val = pid_struct->out_max;
    }
    if (pid_struct->out_min != 0)
    {
        if (out_val < pid_struct->out_min)
            out_val = pid_struct->out_min;
    }
exit:
    return out_val;
}

/*专门为pid自平衡棍弄的*/
float pid_control_gunzi(pid_t *pid_struct, float act_val, short gyro)
{
    float out_val = 0;
    float tar_temp = 0;

    pid_struct->err = pid_struct->tar_val - act_val;

    /*记录上一次实际值和这次实际值*/
    pid_struct->act_last = pid_struct->act;
    pid_struct->act = act_val;

    /*输出死区*/
    if (fabs(pid_struct->err) < pid_struct->out_threshold)
    {

        out_val = 0;
        goto exit;
    }

    /*误差累积*/
    if (pid_struct->ki != 0) // ki有数的情况下才累计误差
    {
        pid_struct->err_sum += pid_struct->err;
    }
    else // ki没数不累计误差,防止猛增
    {
        pid_struct->err_sum = 0;
    }

    /*积分限幅*/
    if (pid_struct->inc_max != 0)
    {
        if (pid_struct->err_sum > pid_struct->inc_max)
            pid_struct->err_sum = pid_struct->inc_max;
    }
    if (pid_struct->inc_min != 0)
    {
        if (pid_struct->err_sum < pid_struct->inc_min)
            pid_struct->err_sum = pid_struct->inc_min;
    }

    pid_struct->dout = -pid_struct->kd * gyro;

    /*输出值计算*/
    out_val = pid_struct->kp * pid_struct->err + pid_struct->ki * pid_struct->err_sum + pid_struct->dout;

    /*输出偏移*/
    if (pid_struct->out_offset > 0)
        out_val += pid_struct->out_offset;
    else if (pid_struct->out_offset < 0)
        out_val -= pid_struct->out_offset;

    /*输出限幅*/
    if (pid_struct->out_max != 0)
    {
        if (out_val > pid_struct->out_max)
            out_val = pid_struct->out_max;
    }
    if (pid_struct->out_min != 0)
    {
        if (out_val < pid_struct->out_min)
            out_val = pid_struct->out_min;
    }

exit:
    return out_val;
}
