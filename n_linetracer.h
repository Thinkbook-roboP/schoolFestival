#pragma once


#include "ev3api.h"
#include <stdio.h>


#include "n_pid.h"
#include "light_sensor.h"

//初期化
void line_init();

//ライントレース(モーター速度、エッジ、目標値)
void n_line_tracing(int power, int edge, int target);
double line_get_control();

/*
//ライン調整
void line_fit(int edge, int target);
//調整終了取得
bool_t line_is_fitted();
*/

