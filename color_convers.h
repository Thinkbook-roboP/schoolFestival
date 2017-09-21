#pragma once

#include "ev3api.h"
#include <math.h>

#include "LCH.h"

//Lchの設定をする(初期化)
void lch_init( rgb_raw_t* );


//lch_raw_t扱いに値を変換する
bool_t to_lch( rgb_raw_t* , lch_raw_t* );


