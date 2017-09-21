#pragma once

#include "ev3api.h"
#include "color_convers.h"
#include "pid.h"


//PIDの係数を設定する
void set_pid_gain(double p_gain, double i_gain, double d_gain);

//ラインとの誤差をモーターの回転量に補正をかけてモーターを回転させる
void line_tracing(double l_value, double pow, int edge);

//両輪のモーターの回転誤差を元により正確な回転をさせる。

//目的速度に本体がぶれないようになめらかに回転する トップスピードになるとtrueを返す

