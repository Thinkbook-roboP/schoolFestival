#pragma once

#include "ev3api.h"


//モードのフラグ条件を設定(初期化)
void set_on_flag_travel();

//シーンに合わせて動作を変更させる
void update_act_state();
