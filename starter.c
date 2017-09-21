#include "starter.h"


bool_t start_stand_by()
{
          char str[32];

          static bool_t Is_pusshed = false;
          bool_t is_push = false;

          is_push = ev3_touch_sensor_is_pressed(EV3_PORT_1);

          if(Is_pusshed && !is_push)
          {
                    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE);
                    return true;
          }
          Is_pusshed = is_push;

          if(Is_pusshed)
                    sprintf(str, "pls leave.");
          else
                    sprintf(str, "pls push.");

          ev3_lcd_draw_string(str, 10, 30);

          return false;
}

