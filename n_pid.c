#include "n_pid.h"

static gein_t *p_gein;   //PIDのポインタ

static int Pid_high, Pid_low, Pid_middle;


//============================================================================
//  PUBLIC
//============================================================================
void n_pid_init(int high, int low)
{
    Pid_high = high;
    Pid_low = low;
    Pid_middle = (Pid_high + Pid_low) / 2.0;
}

void pid_set_gain(gein_t* ptr)
{
    p_gein = ptr;
}


void pid_set_data(gein_t* ptr, double p, double i, double d)
{
    ptr->p_gain = p;
    ptr->i_gain = i;
    ptr->d_gain = d;

}

double pid_get_control(int diff)
{

    const double TIME = 0.001;

    static double Integration = 0;
    static int Past_diff = 0;

    double control_p, control_i, control_d, total;

    control_p  = diff * p_gein->p_gain;    

    Integration += (diff + Past_diff) / 2 * TIME;
    control_i = Integration * p_gein->i_gain;

    control_d = ((diff - Past_diff) * p_gein->d_gain) / TIME;

    total = (control_p + control_i + control_d); 

    Past_diff = diff;


    return total;
}


void pid_get_gein_t(gein_t *_gein)
{
    *_gein = *p_gein;
}


const int PID_HIGH()
{
    return Pid_high;
}

const int PID_LOW()
{
    return Pid_low;
}

const int PID_MIDDLE()
{
    return Pid_middle;
}


