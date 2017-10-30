#include "calibration.h"
#include "base_func.h"
#include "basic_travel.h"
#include "test_get_pid.h"
#include "starter.h"
#include "gray_search.h"


//動作基本

//=================================================================
//private


bool_t switching_mode(int* mode)
{
          bool_t is_return = false;
          int m;
          int dist;
          char str[32];

          switch(*mode)
          {
          case -2:
//                    is_return = init_set_pid_base();
                    is_return = init_state_data();
                    break;

          case -1:
                    is_return = start_stand_by();
                    break;

          case 1:
                    is_return = take_bright_state();
                    break;
          case 2:
                    is_return = gray_search_is_purpose(0, 1, 10, 1);
                    break;
          case 10:
                    is_return = false;
                    ev3_motor_set_power(EV3_PORT_A, 0);
                    ev3_motor_set_power(EV3_PORT_B, 0);
                    ev3_motor_set_power(EV3_PORT_C, 0);
                    ev3_motor_set_power(EV3_PORT_D, 0);

                    dist = ev3_motor_get_counts(EV3_PORT_C);
                    sprintf(str, "DIST >>> %d", dist);
                    ev3_lcd_draw_string(str, 10, 40);

                    break;
          default:
                    //基本走行で、帰ってきた値に合わせて動作を変更する
                    m = basic_traveling();
//                    m = 0;
//                    take_bright_state();
                    if(m != 0)
                              *mode = m;
                    break;
          }

          return is_return;
}

//=================================================================
//public
void update_act_state()
{
          static int mode = 0;

          //モードが変更された場合には、関数を変更する
          if(switching_mode(&mode))
          {
                    mode = 0;
          }

}


void set_on_flag_travel()
{
          int data_num = 6;   //データ個数
          double data[24] = {
                // 速度,P値,I値,D値
                    //20171026
                    // バック後のライントレースのPIDと同じにしたがうまくいかず
                    40, 0.5, 0.1, 0.01,             //S1
                    100, 1.23, 0.1, 0.02,             //S2
                    100, 1.2, 0.1, 0.01,              //S3
                     100, 1.0, 0, 0.0,             //S4
                    100, 1.0, 0, 1.5,             //S5
                    50, 1.2, 0, 1.4             //S6
          };

          /*RコースのPID値は S1　P1.2 I0.1 D0.01
                            S2　P1.23 I0.1 D0.02
                            S3はS1と同様*/
         /*LコースのPID値は
                   */

          basic_init(data_num, data);



//          set_travel_data(0, true, -2, 0);
          set_travel_data(0, true, -1, 0);
          set_travel_data(0, false, 1, 0);  //S1
       //   set_travel_data(8700, false, 1, 1);  // S2  
       //   set_travel_data(10300, false, 1, 2);// S3 


//          gray_search_init();
}


