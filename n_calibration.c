#include "n_calibration.h"

static int L_high = 0, L_low = 0;


//腕の位置を固定化させる
void lock_arm(){
    ev3_motor_set_power(EV3_PORT_A,-80);
    tslp_tsk(1000);
    ev3_motor_stop (EV3_PORT_A,true);

    ev3_motor_reset_counts(EV3_PORT_A);//リセット

    while(ev3_motor_get_counts (EV3_PORT_A)<=35){
        ev3_motor_set_power(EV3_PORT_A,10);
          }
          ev3_motor_stop (EV3_PORT_A,true);
          ev3_motor_reset_counts(EV3_PORT_A);//リセット
}


//画面出力
static void state_out_color(int mode)
{
          switch(mode)
          {
          case 0:
                    ev3_lcd_draw_string("PUSH_TOUCH_SENS", 10, 10);
                    break;
          case 1:
                    ev3_lcd_draw_string("GET_HIGH_RGB___", 10, 10);
                    break;
          case 2:
                    ev3_lcd_draw_string("GET_WHITE______", 10, 10);
                    break;
          case 3:
                    ev3_lcd_draw_string("GET_BLACK______", 10, 10);
                    break;
          default:
                    break;
          }
}

//入力反映
static void input_data_color(int mode, rgb_raw_t* rgb)
{
          static double High;    //明度が最も高い値
          double low;         //明度が最も低い値

          lch_raw_t lch;
          
          switch(mode)
          {
          case 1:
                    //LCH値を初期化する
                    n_lch_init(rgb);
                    lch_init(rgb);
                    break;
          case 2:
                    //白を検出
                    lch_get(&lch);
                    High = lch.l;
                    break;
          case 3:
                    //黒を検出
                    lch_get(&lch);
                    low = lch.l;

                    n_pid_init((int)High, (int)low);
                    pid_init(High, low);

                    break;
          default:
                    break;
          }

}


//PUBLIC/////////////////////////////////////////////////

bool_t n_init_state_data()
{
          static int calib_mode = -1;

          static bool_t Is_pushed_touch = false;  //タッチセンサの入力過去情報
          bool_t is_push_touch;                   //ボタンの現在の情報

          char str[32];


          rgb_raw_t rgb;


          switch(calib_mode)
          {
          case -1:

                    //モーターなどは、初期化しながらほか初期化させる
                    ev3_motor_config(EV3_PORT_A, LARGE_MOTOR);
                    ev3_motor_config(EV3_PORT_B, LARGE_MOTOR);
                    ev3_motor_config(EV3_PORT_C, LARGE_MOTOR);
                    ev3_motor_config(EV3_PORT_D, MEDIUM_MOTOR);

                    //超音波センサ、カラーセンサ、タッチセンサ
                    ev3_sensor_config(EV3_PORT_1, TOUCH_SENSOR);
                    ev3_sensor_config(EV3_PORT_2, COLOR_SENSOR);
                    ev3_sensor_config(EV3_PORT_3, ULTRASONIC_SENSOR);


                    //５秒待機
                    tslp_tsk(5000);

                    //腕の位置を固定化させる
                    lock_arm();

                    calib_mode = 0;
                    L_low = 999;
                    L_high = -999;

                    break;

          case 4:
                    //カラーセンサ系の設定
                    is_push_touch = ev3_button_is_pressed(ENTER_BUTTON);

                    if(!is_push_touch && Is_pushed_touch)
                    {
                              calib_mode = -1;

                              //ブロック並べ生成
                              init_blocksort();


                              Is_pushed_touch = false;

                              return true;        //終了通知
                    }



                    Is_pushed_touch = is_push_touch;        //過去値の更新


                    break;

          default:
                    //カラーセンサ系の設定
                    is_push_touch = ev3_button_is_pressed(ENTER_BUTTON);

                    //ボタン
                    if(!is_push_touch && Is_pushed_touch)
                    {
                              //初期センサー準備と、LCH最大値の取得、設定をする
                              ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb);

                              input_data_color(calib_mode, &rgb);


                              calib_mode ++;
                    }

                    //入力要求テキスト出力
                    state_out_color(calib_mode);

                    Is_pushed_touch = is_push_touch;        //過去値の更新

                    break;

          }


          return false;       //終了していないことを通知


}

