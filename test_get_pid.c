#include "test_get_pid.h"


bool_t take_bright_state()
{
          char str[6][50];
          int i;

          rgb_raw_t rgb;
          lch_raw_t lch;

          ev3_color_sensor_get_rgb_raw(EV3_PORT_2, &rgb);
          to_lch(&rgb, &lch);


          sprintf(str[0], "BRIGHT >>> %f  ", LCH_HIGH());
          sprintf(str[1], "DARK >>> %f  ", LCH_LOW());
          sprintf(str[2], "BORDER >>> %f  ", LCH_MIDDLE());
          sprintf(str[3], "NOW_BAL >>> %f  ", lch.l);
          sprintf(str[4], "SA >>> %f ", LCH_GRAY());
          sprintf(str[5], "SA >>> %f ", LCH_GRAY_MIDDLE());          

          ev3_motor_set_power(EV3_PORT_B, 0);
          ev3_motor_set_power(EV3_PORT_C, 0);

          for(i = 0; i < 6; i++)
          {
                    ev3_lcd_draw_string(str[i], 10, 20 * i);
          }
          return false;
}
