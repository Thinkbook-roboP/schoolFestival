#pragma once

#include "ev3api.h"
#include "analysis.h"
#include "input_code.h"
#include "query_degree.h"
#include "n_pid.h"
#include "n_linetracer.h"
#include "n_light_sensor.h"
#include <math.h>

////////////////////////////////////////////
//****  Private  ****
///////////////////////////////////////

//ブロック情報構造体
typedef struct BLOCK_DATA{
    //色情報
    int color;
    //マップ上の位置
    int position;
    //運搬優先度
    int priority;
    //運搬されたか
    bool_t is_carry;
}block_data;


///////////////////////////////////////////////////////////////////////////////////
//****  Public  ****
//////////////////////////////////////////////////////////////


//cnt番号のブロック構造体の運搬フラグの設定
void set_block_carry(int cnt, bool_t result);
//cnt番号のブロック構造体のいち情報の設定
void set_block_position(int num, int pos);
//cnt番号のブロック構造体の運搬フラグを返す
bool_t get_carry(int cnt);
//cnt番号のブロック構造体の位置を返す
int get_position(int cnt);
//cnt番号のブロック構造体の優先度を返す
int get_priority(int cnt);

//5つ運び終えているか確認
//5つ運び終えた  :true
//         てない:false
bool_t is_carried();


//ブロック並べ内でのPID値初期化
void init_blocksort_gein();


//初期化
void init_blocksort();

//メイン難所攻略
void blocksort();

//終了判定
bool_t block_is_finished();


