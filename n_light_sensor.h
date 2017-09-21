#pragma once

#include "ev3api.h"
#include <math.h>

#include "LCH.h"


void n_lch_init( rgb_raw_t* );  //光センサの初期化

//lch_raw_t扱いに値を変換する
void lch_get( lch_raw_t* );

//lch値より、現在の色を判定する
int lch_color();

void lch_high_color(rgb_raw_t* rgb);
