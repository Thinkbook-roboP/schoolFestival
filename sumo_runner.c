#include "sumo_runner.h"

static FILE *fp;

static gein_t gain;


static bool_t P_is_finished = false;
static bool_t P_is_action_complete = false;

static int P_sumo_tsk = 0;      //相撲タスク

static int P_right_motor_start_counts = 0;
static int P_left_motor_start_counts = 0;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        PRIVATE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//レールのりあげ
static void lail_run_out()
{
          int right, left;

          right = ev3_motor_get_counts(EV3_PORT_C);
          left = ev3_motor_get_counts(EV3_PORT_B);

          if(right >= P_right_motor_start_counts + 700 
                    && left >= P_left_motor_start_counts + 700)
                    P_is_action_complete = true;  //アクション終了フラグを立てる


          ev3_motor_set_power(EV3_PORT_B, 100);
          ev3_motor_set_power(EV3_PORT_C, 100);
}


//距離センサを表示
static void disp_distance()
{
          char str[32];
          int distance;

          if(ev3_button_is_pressed(ENTER_BUTTON))
                    P_is_action_complete = true;

          distance = ev3_ultrasonic_sensor_get_distance(EV3_PORT_3);

          ev3_motor_set_power(EV3_PORT_D, ((distance / 255.0) * 100));

          sprintf(str, "dist >> %d", distance);
          fprintf(fp, "%d\r\n", distance);

          ev3_lcd_draw_string(str, 10, 40);

}


//ゆったりと前進して検出がおかしくなったら終了
static void lain_access()
{
          static bool_t Is_block_access = false,  //ブロック検知異常開始フラグ
                    Is_init = true;     //初期化フラグ

          lch_raw_t lch;

          int distance = ev3_ultrasonic_sensor_get_distance(EV3_PORT_3);

          if(Is_init)
          {
                    //PIDをセット
                    pid_set_data(&gain, 1.2, 0.01, 0.40);
                    Is_init = false;    //初期化完了
          }

          //距離検出
          if(Is_block_access)
          {
                    //200？以上のありえない数値が算出されたら完了を報告
                    if(distance > 100)
                    {
                              Is_block_access = false;
                              P_is_action_complete = true;
                    }
          }
          else
          {
                    //150cm？未満の値を算出したら次の判定条件モードに
                    if(distance < 40)
                              Is_block_access = true;
          }

          //ゆっくりとねっとりとライントレース
          lch_get(&lch);
          n_line_tracing(50, 1, PID_MIDDLE());

}


//=====================================================================================
//        PUBLIC
//=====================================================================================

//初期化
void sumo_init()
{
          if((fp = fopen("DISTANCE.txt", "w")) == NULL)
                    P_is_finished = true;

          P_is_finished = false;        //終了フラッグを下ろす
          P_is_action_complete = false; //動作フラグを下ろす

          pid_set_data(&gain, 0.0, 0.0, 0.0);

          pid_set_gain(&gain);

          P_sumo_tsk = 0;     //相撲タスク番号

          ev3_motor_reset_counts(EV3_PORT_B);
          ev3_motor_reset_counts(EV3_PORT_C);

          P_left_motor_start_counts = ev3_motor_get_counts(EV3_PORT_B);     //左モータ初期位置を保存
          P_right_motor_start_counts = ev3_motor_get_counts(EV3_PORT_C);    //右モータ初期位置を保存
}

//動作
void sumo_running()
{
          switch(P_sumo_tsk)
          {
          case 1:
                    lail_run_out();
                    break;
          case 2:
                    disp_distance();
                    break;
          case 0:
                    lain_access();
                    break;
          default:
                    break;
          }

          if(P_is_action_complete)
          {
                    P_is_action_complete = false; //アクション完了フラグを下ろす
                    P_sumo_tsk ++;        //タスクを次に移す

                    if(P_sumo_tsk > 0)    P_is_finished = true;
          }

}

//終了
bool_t sumo_is_finished()
{
          //終了フラグが立っている場合終了
          if(P_is_finished)
          {
                    fclose(fp);
                    P_is_finished = false;        //終了フラグを下ろす
                    return true;
          }
          return false;
}

