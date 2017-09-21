#include "app.h"
#include "color_convers.h"
#include "base_func.h"

#include "n_calibration.h"
#include "operate_course.h"

int touch_sensor = EV3_PORT_1, color_sensor = EV3_PORT_2, ultra_sensor = EV3_PORT_3, gyro_sensor = EV3_PORT_4,
  arm_motor = EV3_PORT_A, right_motor = EV3_PORT_B, left_motor = EV3_PORT_C, tail_motor = EV3_PORT_D;

void display_value(lch_raw_t* val, rgb_raw_t* rgb);


/* 高優先度タスク */
void task1(intptr_t exinf)
{

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
  while(ev3_motor_get_counts(EV3_PORT_C) <= 10)//14400
  {
    update_act_state();

  }


  while(!operate_is_finished())
  {
    operate_running();
  }

  ev3_motor_set_power(EV3_PORT_C, 0);
  ev3_motor_set_power(EV3_PORT_B, 0);

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
 
