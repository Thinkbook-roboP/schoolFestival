#pragma once

#include "pid.h"
#include "motor_encode.h"
#include "color_convers.h"


//初期化(何かあったかしら。)
void gray_search_init();


//灰色レールに乗ったもしくは、通常ラインに復帰した場合に終了
//灰色検知(g_curve:0~3, end_type:0>灰色検知後に終了,1>灰色検知し終わった際に終了)
bool_t gray_search_is_purpose(int g_curve, int end_type, int pow, int edge);

