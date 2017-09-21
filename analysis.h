#pragma once

#include "ev3api.h"




//



///////////////////////////////////////////////////////////////////////////////////
//Public
//黒ブロックの位置を返す
int get_position_black();
//赤ブロックの位置を返す
int get_position_red();
//黄ブロックの位置を返す
int get_position_yellow();
//青ブロックの位置を返す
int get_position_blue();

bool_t analysis(int code);

