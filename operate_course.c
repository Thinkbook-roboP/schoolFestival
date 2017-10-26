#include "operate_course.h"

static const int OUT_MODE = -1000;

static gein_t Gein;

static int R_count = 0;

static operate_t _operate[20];
static int Operate_count = 0;

static int Judge_mode = 0, Action_mode = 0, Next_mode = 0;

static bool_t Is_finished = false;


void operate_set_data(int num, int r_count, bool_t is_special, int task_num)
{
    _operate[num].motor_count = r_count;

    _operate[num].is_special_event = is_special;
    _operate[num].set_num = task_num;
}


void operate_motor_pow(int number)
{
    switch(number)
    {
    case 1:
        //高速
        pid_set_data(&Gein, 1.3, 0.001,1.0);
        break;
    case 2:
        //中速
        pid_set_data(&Gein, 1.3, 0.001,1.0);
        break;
    case 3:
        pid_set_data(&Gein, 1.3, 0.001,0.5);//ブロック並べまでのバトン
        break;
    case 11:
        //低速
        pid_set_data(&Gein, 0, 0.0, 0.0);
        break;
    case 12:
        pid_set_data(&Gein, 0, 0.0, 0);
        break;
    case 13:
        pid_set_data(&Gein,0,0.0,0.0);
        break;
    default:
        break;
    }
}

static void operate_judge()
{
    switch(Judge_mode)
    {
    case -1:
        Is_finished = true;
        break;

    case 0:
        //距離測定
        R_count = ev3_motor_get_counts(EV3_PORT_C);
        if(R_count >= _operate[Operate_count].motor_count)
        {
//            ev3_motor_reset_counts(Motor->right_motor);
//            ev3_motor_reset_counts(Motor->left_motor);

            if(_operate[Operate_count].is_special_event)
            {
                //難所番号を入力する
                Judge_mode = _operate[Operate_count].set_num;
                Next_mode = _operate[Operate_count].set_num;
                Action_mode = OUT_MODE;
            }
            else
            {
                //通常走行の走行スピードとPIDを調整する
                operate_motor_pow(_operate[Operate_count].set_num);
            }

            Operate_count ++;
        }
        //状態設定
        break;
    case 1:
        // *************** ブロック並べ終了判定************
        if(block_is_finished()){
            Judge_mode = OUT_MODE;
            Action_mode = OUT_MODE;
        }
        break;

    default:
        //PIDの値を戻す
        pid_set_gain(&Gein);

        Judge_mode = 0;
        Action_mode = 0;
        
        break;
    }
}

static void operate_action()
{
    switch(Action_mode)
    {
    case -1:
        ev3_motor_set_power(EV3_PORT_A, 0);
        ev3_motor_set_power(EV3_PORT_B, 0);
        ev3_motor_set_power(EV3_PORT_C, 0);
        ev3_motor_set_power(EV3_PORT_D, 0);

        break;

    case 0:
        n_line_tracing(30,-1, PID_MIDDLE());
        break;

    case 1:
        //ブロック並べ
        blocksort();
        break;



    default:
        switch(Next_mode)
        {        
        case 1:
            //ブロック並べPID値初期化
            init_blocksort_gein();
            break;
        default:
            break;
        }
        Action_mode = Next_mode;        //アクションモード更新
        Next_mode = 0;  //次モードを初期化する

        break;
    }
}

//=============================================================================================================

//初期化
void operate_init()
{
    line_init();

    //情報設定
    //直進
 //  operate_set_data(0, 3000, false, 2);  //高速、直線にフィットする
//    operate_set_data(0, 0, true, 1);
    operate_set_data(0, 2700, true, 1);
    operate_set_data(1, 0, true, -1);
//    operate_set_data(0, 0, true, 100);
  

//    pid_set_data(&Gein, 1.45, 0.000, 1.2);

    operate_motor_pow( 3 );

    pid_set_gain(&Gein);

    Operate_count = 0;

}

void operate_running()
{
    char str[32];

    operate_judge();
    operate_action();

    sprintf(str, "MODE:%d,%d", Judge_mode, Action_mode);
    ev3_lcd_draw_string(str, 10, 40);
}

bool_t operate_is_finished()
{
    if(Is_finished)
    {
        Is_finished = false;
        return true;
    }
    return false;
}


