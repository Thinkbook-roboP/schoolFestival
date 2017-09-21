#pragma once

#include "ev3api.h"
#include "motor_encode.h"
#include "pid.h"


//次のステート、キョリ、ステート設定（０:通常 １:難所設定）、エッジ、PID設定番号
typedef struct TRAVEL_DATA
{
          int distance;
          bool_t is_high_mode;
          int edge;
          int pid_mode;
}travel_data;



//走行をする際の速度と調整量を受け取った値に準じて初期化する。
//POWER, P, I, D, ~~~~のような形で所持した配列を渡してください。
void basic_init(int number, double *pid_data);


//ステートをセットする。指定した分だけ続いていく
void set_travel_data(int dist, bool_t is_high, int edge, int pid_mode);

//基本走行時にイベントの進行に合わせてモータの回転速度を調整、別のモードにいれるなどをする
//1:アーム 2:左モータ 3:右モータ 4:しっぽモータ
int basic_traveling();

