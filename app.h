
/*
 *  ターゲット依存の定義
 */
 #include "ev3api.h"
#include "target_test.h"

/*
 *  各タスクの優先度の定義
 */

#define MAIN_PRIORITY	5		/* メインタスクの優先度 */
								/* HIGH_PRIORITYより高くすること */

#define HIGH_PRIORITY	9		/* 並行実行されるタスクの優先度 */
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

/*
 *  ターゲットに依存する可能性のある定数の定義
 */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF		ULONG_C(1000000)	/* 速度計測用のループ回数 */
#endif /* LOOP_REF */



typedef struct _EV3_VALUE_SENSOR
{
	bool_t 		is_touched;	//タッチセンサ
	rgb_raw_t		val_color;	//カラーセンサー
	int 		val_ultra;	//超音波センサー
	//ジャイロセンサ（？）これいるかしら
}e_sensor;

typedef struct _EV3_ANGLE_MOTOR
{
	int right;//右モーター
	int left;	//左モーター
	int arm;	//腕モーター
	int tail;	//しっぽモーター
}e_angle_motor;

//EV3内で取り扱えれる物理情報
typedef struct _EV3_PHYSIC_STATE 
{
	e_sensor		s_value;//センサー値
	e_angle_motor	m_angle;//モーター各、回転角
}e_physic;


/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY



void task1(intptr_t exinf);	

extern void input_outside_aq();	//モーター角度、センサー値を取得
extern void update();		//情報確認、モーター回転量の更新
extern void motor_rotation();		//モーター回転

void display(int);
//ulong_t getTime();


#endif /* TOPPERS_MACRO_ONLY */
