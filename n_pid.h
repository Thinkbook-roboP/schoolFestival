#pragma once

#include "ev3api.h"

typedef struct N_PID_RAW_T
{
    double p_gain;
    double i_gain;
    double d_gain;
} gein_t;


void n_pid_init(int high, int low);  //使用するpid値を持つ
void pid_set_gain(gein_t* ptr);
void pid_set_data(gein_t* ptr, double p, double i, double d);    //PIDのゲイン値を設定する
double pid_get_control(int diff);   //制御値を返す(誤差値)
void pid_get_gein_t(gein_t *_gein);


const int PID_HIGH();
const int PID_LOW();
const int PID_MIDDLE();


