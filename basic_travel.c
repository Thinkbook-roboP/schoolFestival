#include "basic_travel.h"

static travel_data actual_value[20];
static bool_t Is_hard_mode = false;
static double action_speed[40];
static double motor_pow = 0;
static int max_actual = 0;
static int mode_count = 0;
static int edge;

//PRIVATE////////////////////////////////////////////////////////////

//PIDナンバーに合わせて対応したPID補正値をわたし、PIDゲインを更新させる

void pid_setter(int num)
{
          motor_pow = action_speed[num * 4];
          set_pid_gain(action_speed[num * 4 + 1], 
                    action_speed[num * 4 + 2],
                    action_speed[num * 4 + 3]);
}


//モードに合わせてステートを変更する
int change_mode(int mot_angle)
{
          //要求されるタスク数よりも多くならないようにする
          if(mode_count >= max_actual)
                    return -1;

          if(actual_value[mode_count].distance <= mot_angle)
          {
                Is_hard_mode = actual_value[mode_count].is_high_mode;      //次動作がライントレースかどうかを取得
                
                 //モードを次に移す
                    if(actual_value[mode_count].is_high_mode)
                    {
                              //難所呼び出し番号を返す。
                              return actual_value[mode_count++].edge;
                    }

                    edge = actual_value[mode_count].edge;
                    pid_setter(actual_value[mode_count].pid_mode);

                    mode_count++;       //モードを次に移す
                    //動作変更がないため、０を返す
          }

          return 0;
          
}




//PUBLIC/////////////////////////////////////////////////////////////

void basic_init(int number, double *pid_data)
{
          int i;
          for(i = 0; i < number * 4; i++)
                    action_speed[i] = *pid_data++;
}



void set_travel_data(int dist, bool_t is_high, int edge, int pid_mode)
{
          actual_value[max_actual].distance = dist;
          actual_value[max_actual].is_high_mode = is_high;
          actual_value[max_actual].edge = edge;
          actual_value[max_actual].pid_mode = pid_mode;

          if(max_actual == 0)
            Is_hard_mode = is_high;

          max_actual ++;
}


int basic_traveling()
{
          bool_t is_un_line_tracing_mode = false;
          int mot_counts, return_mode_num;
          int dist;

          rgb_raw_t rgb;
          lch_raw_t lch;


          //ライントレースフラグはこっちにするのかもしれない。

          //モータの角度を検出   
          mot_counts = ev3_motor_get_counts(EV3_PORT_C);    
          return_mode_num = change_mode(mot_counts);


          //ライントレースモード時のみ動作をさせる
          if(!Is_hard_mode)
          {
                    //色から、輝度を求める
                    ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb);
                    to_lch(&rgb, &lch);

                    dist = ev3_ultrasonic_sensor_get_distance(EV3_PORT_3);
                    if(dist <= 10)
                        return 10;


                    line_tracing(lch.l, motor_pow, edge);

          }


          return return_mode_num;
}

