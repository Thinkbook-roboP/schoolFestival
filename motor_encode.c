#include "motor_encode.h"

static pid_raw_t gain;

static bool_t Is_first = true;

//PIDの係数を設定する
void set_pid_gain(double p_gain, double i_gain, double d_gain)
{
          //初回フラグが立っている場合は、下ろす。
          if(Is_first)        Is_first = false;

          gain.p_gain = p_gain;
          gain.i_gain = i_gain;
          gain.d_gain = d_gain;
}


//ラインの誤差を補正して走行(edge = 1 :　右)
void line_tracing(double l_value, double pow, int edge)
{
          double pid, diff;

          //初回フラグが立っている場合は、数値を０に初期化してフラグを下ろす
          if(Is_first)
          {
                    set_pid_gain(0, 0, 0);
                    Is_first = false;

          }

          diff = LCH_MIDDLE() - l_value;       //誤差を求める

          pid = pid_get_adjust_value(diff, &gain);
/*
          //右モータ
          ev3_motor_set_power(EV3_PORT_B, (int)(pow + ((pid * edge) * 0)));
          //左モータ
          ev3_motor_set_power(EV3_PORT_C, (int)(pow - ((pid * edge) * 0)));
*/

          //左モータ
          ev3_motor_set_power(EV3_PORT_B, pow - (pid * edge));
          //右モータ
          ev3_motor_set_power(EV3_PORT_C, pow + (pid * edge));

}


