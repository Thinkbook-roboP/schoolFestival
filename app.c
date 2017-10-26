#include "app.h"
#include "color_convers.h"
#include "base_func.h"

#include "n_calibration.h"
#include "operate_course.h"

int touch_sensor = EV3_PORT_1, color_sensor = EV3_PORT_2, ultra_sensor = EV3_PORT_3, gyro_sensor = EV3_PORT_4,
  arm_motor = EV3_PORT_A, right_motor = EV3_PORT_B, left_motor = EV3_PORT_C, tail_motor = EV3_PORT_D;

void display_value(lch_raw_t* val, rgb_raw_t* rgb);

int J_judge_black(void);


/* 高優先度タスク */
void task1(intptr_t exinf)
{
 // rgb_raw_t rgb;
  static gein_t gein;

  ulong_t now_time, first_time;
  int i = 0;

  char message1[50];


  //センサー初期化
  while(ev3_sensor_config(touch_sensor, TOUCH_SENSOR) != E_OK);
  while(ev3_sensor_config(color_sensor, COLOR_SENSOR) != E_OK);
  while(ev3_sensor_config(ultra_sensor, ULTRASONIC_SENSOR) != E_OK);

  //モーターの初期化
  while(ev3_motor_config(right_motor, LARGE_MOTOR) != E_OK);
  while(ev3_motor_config(left_motor, LARGE_MOTOR) != E_OK);
  while(ev3_motor_config(arm_motor, LARGE_MOTOR) != E_OK);
  while(ev3_motor_config(tail_motor, MEDIUM_MOTOR) != E_OK);

  while(!n_init_state_data());

  set_on_flag_travel();
  operate_init();

  ev3_lcd_set_font(EV3_FONT_MEDIUM);

  //ここらへんが処理されて死亡
  while(ev3_motor_get_counts(EV3_PORT_C) <= 14400)//14400
  {
    update_act_state();

  }

//180ターンする
ev3_motor_set_power(EV3_PORT_B,0);
ev3_motor_set_power(EV3_PORT_C,0);

ev3_motor_reset_counts(EV3_PORT_B);
ev3_motor_reset_counts(EV3_PORT_C);
 
ev3_motor_set_power(right_motor, -40);
ev3_motor_set_power(left_motor, 40);
while (abs(ev3_motor_get_counts(right_motor)) < 260);

ev3_motor_set_power(EV3_PORT_B,0);
ev3_motor_set_power(EV3_PORT_C,0);

pid_set_data(&gein,0.5,0.0,1);
pid_set_gain(&gein);

while(ev3_motor_get_counts(EV3_PORT_B)<=3700){
  n_line_tracing(20,1,PID_MIDDLE()+15);
}

//ストップ
ev3_motor_stop(EV3_PORT_B, true);
ev3_motor_stop(EV3_PORT_C, true);

//縦列駐車開始
/**************************
 * ここで駐車の調整
 ***************************/
ev3_motor_set_power(EV3_PORT_A,50);
  
tslp_tsk(800);//アームを上げる
ev3_motor_stop(EV3_PORT_A,true);
// ev3_motor_set_power(EV3_PORT_C,50);

// tslp_tsk(320);//5秒右回転
  

ev3_motor_set_power(EV3_PORT_B,10); //モータを10で回し続ける
ev3_motor_set_power(EV3_PORT_C,10); 


tslp_tsk(6500);//直進

ev3_motor_stop(EV3_PORT_C,true);
ev3_motor_set_power(EV3_PORT_B,50);
//  ev3_motor_set_power(EV3_PORT_B,10);

tslp_tsk(500);//左回転

//  ev3_motor_stop(EV3_PORT_B,true);
ev3_motor_stop(EV3_PORT_B,true);
  /*--------------ここまで--------------------------*/

  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  sprintf(message1, ">>>>%d", i);
  ev3_lcd_draw_string(message1, 10, 70);

}

void display(int num)
{
  char message[7][32];

  sprintf(message[0], "LOADING .");
  sprintf(message[1], "LOADING ..");
  sprintf(message[2], "LOADING ...");
  sprintf(message[3], "COMPLEAT!!!");
  sprintf(message[4], "FINISHED.");
  sprintf(message[5], "WAIT_PUSH.");

  ev3_lcd_set_font(EV3_FONT_MEDIUM);
  ev3_lcd_draw_string(message[num], 20, 30);

}
int J_judge_black(void){
  lch_raw_t g;
  
   //g.l明るさ(明度)、g.c彩度、g.h色相角
   
       lch_get(&g);
     //  tslp_tsk(100);
     //  printf("L:%f,C:%f,h:%f\n",g.l,g.c,g.h);
    //   printf("V:%d\n",ev3_battery_voltage_mV());
    //   tslp_tsk(900);

    if(g.l<=30 && g.l>=0 && g.c<=10){//黒の場合1を返す
      return 1;
    }
    return 0;
   
}
 
