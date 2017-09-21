#pragma once

#include "ev3api.h"

#include "light_sensor.h"

#include "n_pid.h"
#include "n_linetracer.h"
#include "blocksort.h"



typedef struct
{
    int motor_count; //0:右モータ
    bool_t is_special_event;    //エッジ変更や難所モードか(T:成立　F:非成立)
    int set_num;    //動作番号(通常モードで1~3,　エッジなどで1~3)
}operate_t;

//初期化
void operate_init();

//実行
void operate_running();

//終了フラグ
bool_t operate_is_finished();
