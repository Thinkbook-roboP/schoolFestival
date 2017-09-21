#include "gray_search.h"

static pid_raw_t gain;
static lch_raw_t lch;

static bool_t Is_init = false;
static bool_t Is_gray_checking = false;
static bool_t Is_gray_checked = false;


//==========================================================================================
//PRIVATE

static void gray_pid_init(int g_curve)
{
          //PID値のパラメータを決定
          if(!Is_init)
          {         
                    //現在のPID値を保存しておき、検知終了時にPIDを戻す。が、とりあえず使わない
                    
                    //カーブタイプに合わせてPIDを設定する
                    switch(g_curve)
                    {
                    case 0:
                              gain.p_gain = 0.0;
                              gain.i_gain = 0.0;
                              gain.d_gain = 0.0;
                              break;
                    case 1:
                              gain.p_gain = 0.0;
                              gain.i_gain = 0.0;
                              gain.d_gain = 0.0;
                              break;
                    case 2:
                              gain.p_gain = 0.0;
                              gain.i_gain = 0.0;
                              gain.d_gain = 0.0;
                              break;
                    case 3:  
                              gain.p_gain = 0.0;
                              gain.i_gain = 0.0;
                              gain.d_gain = 0.0;
                              break;
                    default:
                              gain.p_gain = 0.0;
                              gain.i_gain = 0.0;
                              gain.d_gain = 0.0;
                              break;
                    }
                    Is_init = true;
          }
}

static bool_t gray_search_is_finished(int end_type)
{
          //判定する条件を選択
          switch(end_type)
          {
          case 0:
                    //灰色検知時に終了
                    if(Is_gray_checked)
                    {
                              gray_search_init();
                              return true;
                    }
                    break;
          case 1:
                    //灰色検知終了時に終了
                    //灰色検出判定がある上で、
                    if(Is_gray_checked)
                    {
                              //中間値以下になって黒が介入してきたことを判断した
                              if(lch.l <= LCH_MIDDLE())
                              {
                                        gray_search_init();
                                        return true;
                              }
                    }
                    break;
          default:
                    break;
          }

          return false;

}



//==========================================================================================
//PUBLIC

void gray_search_init()
{
          Is_init = false;
          Is_gray_checking = false;
          Is_gray_checked = false;

          gain.p_gain = 0.0;
          gain.i_gain = 0.0;
          gain.d_gain = 0.0;

}


bool_t gray_search_is_purpose(int g_curve, int end_type, int pow, int edge)
{
//          static ulong_t Timer_push = 0;          //タイマーが開始した時間

          rgb_raw_t rgb;
//          ulong_t timer_now = 0;                       //タイマーが回っている時間

          double diff, pid;

          char str[32];

//          gray_pid_init(g_curve);

          

          //rgb値の取得、Lchに変換
          ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb);
          to_lch(&rgb, &lch);
          
/*

          //灰色検知確認
          if(LCH_GRAY() <= lch.l && LCH_GRAY_MIDDLE() >= lch.l)
          {
                    get_tim(&timer_now);
                    if(!Is_gray_checking)
                              Timer_push = timer_now;

                    //灰色を検出している時間が0.3秒以上だった場合灰色を検出したとみなす
                    if(timer_now - Timer_push >= 300)
                              Is_gray_checked = true;


                    Is_gray_checking = true;
          }
          else      Is_gray_checking = false;
*/

          //ライントレース ----
          //灰色非検知状態、検知状態に合わせて基準値を、白黒、白灰に合わせる。
//          if(Is_gray_checked)
//                    diff = LCH_GRAY_MIDDLE() - lch.l;     //検知状態
//          else
                    diff = LCH_MIDDLE() - lch.l;  //非検知状態


          pid = pid_get_adjust_value(diff, &gain);     //PID調整量



          //左モータ
          ev3_motor_set_power(EV3_PORT_B, pow - (pid * edge));
          //右モータ
          ev3_motor_set_power(EV3_PORT_C, pow + (pid * edge));
          //----


          sprintf(str, "p>>>%f___i>>>%f___d>>>%f___", gain.p_gain, gain.i_gain, gain.d_gain);
          ev3_lcd_draw_string(str, 10, 40);

          //灰色検知状態が指定されたルールに条件が一致していた場合trueを返す
          return gray_search_is_finished(end_type);
}

