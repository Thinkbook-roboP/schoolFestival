#pragma once
#include "ev3api.h"


typedef struct PID_RAW_T
{
          double p_gain;
          double i_gain;
          double d_gain;
}pid_raw_t;



//PID値で用いる値を初期設定する
void pid_init(double lch_high, double lch_low);


//PIDから操作量を算出する(pid_value.c)
double pid_get_adjust_value(double l, pid_raw_t* gain);


//明度の最大値と、最小値、閾値など
const double LCH_HIGH();
const double LCH_LOW();
const double LCH_MIDDLE();
const double LCH_GRAY();
const double LCH_GRAY_MIDDLE();

