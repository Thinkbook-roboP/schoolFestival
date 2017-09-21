#include "n_linetracer.h"


static bool_t Is_fit_finished = false;

static double Control_pow = 0;

void line_init()
{
    Is_fit_finished = false;
    Control_pow = 0;
}

void n_line_tracing(int power, int edge, int target)
{
    lch_raw_t lch;

    char str[100];
    int diff;
    int right, left;

    lch_get(&lch);
    diff = target - lch.l;

    Control_pow = pid_get_control(diff);

    right = power + Control_pow * -1;
    left = power - Control_pow * -1;

    if(right > 100) right = 100;
    else if(right < -100) right = -100;
    if(left > 100)  left = 100;
    else if(left < -100) left = -100;

    ev3_motor_set_power(EV3_PORT_C, right);
    ev3_motor_set_power(EV3_PORT_B, left);

//    fprintf(fp, "%d,%f\r\n", diff, Control_pow);

//    sprintf(str, "RIGHT >>>> ___%d___", right);    
//    ev3_lcd_draw_string(str, 10, 100);
}

double line_get_control()
{
    return Control_pow;
}

/*
void line_fit(int edge, int target)
{
    int reflect = light_reflect();
    int diff = target - reflect;

    if(diff <= 10 && diff >= -10)
        Is_fit_finished = true;

    int control_pow = pid_get_control(diff);

    ev3_motor_set_power(motor_port->right_motor, (control_pow * edge));
    ev3_motor_set_power(motor_port->left_motor, -(control_pow * edge));
}

bool_t line_is_fitted()
{
    if(Is_fit_finished)
    {
        Is_fit_finished = false;
        return true;
    }
    return false;
}

*/