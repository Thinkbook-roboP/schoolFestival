#include "blocksort.h"

//  *****  ライントレースの弄り方♂♂  *****
//　・速さ
//      blocksort()の中
//          n_line_tracing()の引数
//  ・PID
//      init_blocksort_gein()の中
//          pid_set_data()の引数
//TODO Pの値下げる
//  **********************************

/////////////////////////////////////////////////////////////////////
//*************   private   ****************
//ブロック構造体
//黒～赤～黄～青～緑
static block_data block[5];

//pid用
static gein_t gein;

//マップメモリ
static int map[16];

//ルート
static int route[12][6];
//ルート(縦)
static int vertical;
//ルート(横)
static int horizon;
//前のマス
static int position_pass;
//現在地
static int position_now;
//次のマス
static int position_next;

//ブロック並べモード
// 0:移動
// 1:運搬
static int bsmode;

//ブロック並べ
static bool_t is_finished;

//モーター設定
static int left_motor = EV3_PORT_B, right_motor = EV3_PORT_C;

//画面出力
void show_text(){
    //LCD出力用char
    char text[50];
    ev3_lcd_set_font(EV3_FONT_MEDIUM);
    sprintf(text, "Black>>%02d Red>>%02d", block[0].position, block[1].position);
    ev3_lcd_draw_string(text, 0, 70);
    sprintf(text, "Yello>>%02d Blue>>%02d", block[2].position, block[3].position);
    ev3_lcd_draw_string(text, 0, 90);
    sprintf(text, "Green>>%02d", block[4].position);
    ev3_lcd_draw_string(text, 0, 110);
}

//***  色情報入力  ***
void input_color()
{
    //ブロック色情報入力
    block[0].color = COLOR_BLACK;
    block[1].color = COLOR_RED;
    block[2].color = COLOR_YELLOW;
    block[3].color = COLOR_BLUE;
    block[4].color = COLOR_GREEN;
}

//***  位置情報入力  ***
void input_position()
{

    //初期位置コード解析
    int code = input_code();
    analysis(code);
    ///*
    block[0].position = get_position_black();
    block[1].position = get_position_red();
    block[2].position = get_position_yellow();
    block[3].position = get_position_blue();
    //*/
    /*  手打ちモード
    block[0].position = 4;
    block[1].position = 9;
    block[2].position = 6;
    block[3].position = 3;
    */
    //緑の位置は当日決まる
    block[4].position = 13;

    //画面確認
    printf("Position:");
    for (int i = 0; i < 5; i++)
    {
        printf("%d,", block[i].position);
    }
    printf("\n");
}

//***  優先度情報入力  ***
void input_priority()
{
    printf("Priority__Init\n");
    //優先度初期化
    for (int i = 0; i < 5; i++)
    {
        block[i].priority = 0;
        printf("%d,", block[i].priority);
    }
    printf("\n");

    //*** 五角形内にあるか確認 ***
    printf("Priority_PLUS\n");
    //五角形の位置の構成要素
    int penta[5] = {6, 8, 9, 13, 14};
    for (int ib = 0; ib < 5; ib++)
    {
        for (int ip = 0; ip < 5; ip++)
        {
            if (block[ib].position == penta[ip])
            {
                //ブロック位置と五角形エリアと一致するなら+1
                block[ib].priority += 1;
            }
        }
        printf("%d,", block[ib].priority);
    }
    printf("\n");
}

//マップの色情報入力
void init_map(){
    map[1] = COLOR_RED;
    map[2] = COLOR_BLUE;
    map[3] = COLOR_YELLOW;
    map[4] = COLOR_BLUE;
    map[5] = COLOR_YELLOW;
    map[6] = COLOR_GREEN;
    map[7] = COLOR_RED;
    map[8] = COLOR_RED;
    map[9] = COLOR_BLUE;
    map[10] = COLOR_GREEN;
    map[11] = COLOR_GREEN;
    map[12] = COLOR_BLUE;
    map[13] = COLOR_YELLOW;
    map[14] = COLOR_RED;
    map[15] = COLOR_YELLOW;
}

void stop_and_reset(){
    //モーター停止
    ev3_motor_set_power(EV3_PORT_C, 0);
    ev3_motor_set_power(EV3_PORT_B, 0);
    //カウントリセット
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(left_motor);
}

//whileで完結  回転中心マスにブロックがない場合
void rotate(int degree){

    stop_and_reset();

    switch (abs(degree))
    {
    case 0:
        //直進
        //カウントリセット
        ev3_motor_reset_counts(right_motor);
        ev3_motor_reset_counts(left_motor);
        //直進
        while (ev3_motor_get_counts(right_motor) < 150)
        {
            ev3_motor_set_power(right_motor, 40);
            ev3_motor_set_power(left_motor, 40);
        }
        //カウントリセット
        ev3_motor_reset_counts(right_motor);
        ev3_motor_reset_counts(left_motor);
        break;
    case 30:
        if (degree > 0)
        {
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 7)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 90)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 7)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 170)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }

        break;
    case 45:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 15)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 90)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 13)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 170)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 60:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 20)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 70)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 30)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 170)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 90:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 55)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 90)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 80)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 170)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 105:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 90)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 105)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 120)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 160)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 120:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 110)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 85)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 140)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 130)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 135:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 130)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 85)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 200)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 130)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 150:
        if (degree > 0)
        {
            //右回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 180)
            {
                ev3_motor_set_power(right_motor, -30);
                ev3_motor_set_power(left_motor, 60);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 60)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        else
        {
            //左回転
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //右回転
            while (abs(ev3_motor_get_counts(right_motor)) < 250)
            {
                ev3_motor_set_power(right_motor, 60);
                ev3_motor_set_power(left_motor, -30);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
            //直進
            while (ev3_motor_get_counts(right_motor) < 130)
            {
                ev3_motor_set_power(right_motor, 40);
                ev3_motor_set_power(left_motor, 40);
            }
            //カウントリセット
            ev3_motor_reset_counts(right_motor);
            ev3_motor_reset_counts(left_motor);
        }
        break;
    case 180:
        //右回転
        ev3_motor_set_power(right_motor, -40);
        ev3_motor_set_power(left_motor, 40);
        while (abs(ev3_motor_get_counts(right_motor)) < 280);

        break;
    default:
        if (degree > 0)
        {
            //右回転
        }
        else
        {
            //左回転
        }
        printf("****ERROR****\n");
        while (1)
        {
        }
        break;
    }

    stop_and_reset();
}

//回転中心マスにブロックがある場合
//仕様上移動時のみに利用
//ブロック運搬では呼び出されない(はず)
void escape(int degree){
    
    //****全体の流れ****
    //1.バック
    //2.右か左に45度回転
    //3.ブロックを避けるようにブロックを中心の弧を描くように回転
    //4.右エッジになるように調整

    stop_and_reset();

    //1.バック
    ev3_motor_set_power(right_motor, -40);
    ev3_motor_set_power(left_motor, -40);
    while (abs(ev3_motor_get_counts(right_motor)) < 70);
    
    stop_and_reset();

    //2.左右どちらかに回転
    if(degree == 180){
        //何もしない
    }else if(degree > 0){
        //右に45度くらいその場で回転
        ev3_motor_set_power(right_motor, -20);
        ev3_motor_set_power(left_motor, +20);
        while(abs(ev3_motor_get_counts(right_motor)) < 120);
    }else{
        //左に45度くらいその場で回転
        ev3_motor_set_power(right_motor, +20);
        ev3_motor_set_power(left_motor, -20);
        while(abs(ev3_motor_get_counts(right_motor)) < 70);
    }
    
    stop_and_reset();

    
    //3.ブロックを避けるようにブロックを中心の弧を描くように回転
    switch (abs(degree)){
    case 0:
        //回転
        ev3_motor_set_power(right_motor, 10);
        ev3_motor_set_power(left_motor,  20);
        while (abs(ev3_motor_get_counts(right_motor)) < 450);

        stop_and_reset();

        //線復帰のための調整
        ev3_motor_set_power(right_motor, 20);
        while(abs(ev3_motor_get_counts(right_motor)) < 150);

        stop_and_reset();

        break;
    case 30:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 650);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 200);

            stop_and_reset();

        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 350);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            while(abs(ev3_motor_get_counts(right_motor)) < 150);

            stop_and_reset();

        }

        break;
    case 45:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 520);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 160);

            stop_and_reset();

        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 300);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            while(abs(ev3_motor_get_counts(right_motor)) < 150);

            stop_and_reset();

        }
        break;
    case 60:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 450);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 160);

            stop_and_reset();

        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 250);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            while(abs(ev3_motor_get_counts(right_motor)) < 150);

            stop_and_reset();

        }
        break;
    case 90:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 340);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 190);

            stop_and_reset();

        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 190);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            while(abs(ev3_motor_get_counts(right_motor)) < 240);

            stop_and_reset();

        }
        break;
    case 105:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 250);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 240);

            stop_and_reset();

        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 140);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            while(abs(ev3_motor_get_counts(right_motor)) < 240);

            stop_and_reset();
        }
        break;
    case 120:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 170);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 240);

            stop_and_reset();

        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 100);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            while(abs(ev3_motor_get_counts(right_motor)) < 250);

            stop_and_reset();
        }
        break;
    case 135:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 110);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor, -5);
            while(abs(ev3_motor_get_counts(left_motor)) < 250);

            stop_and_reset();
        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 80);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor, -10);
            while(abs(ev3_motor_get_counts(right_motor)) < 180);

            stop_and_reset();
        }
        break;
    case 150:
        if (degree > 0)
        {
            //右回転
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor,  10);
            while (abs(ev3_motor_get_counts(right_motor)) < 40);

            stop_and_reset();
            
            //線復帰のための調整
            ev3_motor_set_power(left_motor, 20);
            ev3_motor_set_power(right_motor,-20);
            while(abs(ev3_motor_get_counts(left_motor)) < 120);
        }
        else
        {
            //左回転
            ev3_motor_set_power(right_motor, 10);
            ev3_motor_set_power(left_motor,  20);
            while (abs(ev3_motor_get_counts(right_motor)) < 70);

            stop_and_reset();

            //線復帰のための調整
            ev3_motor_set_power(right_motor, 20);
            ev3_motor_set_power(left_motor, -10);
            while(abs(ev3_motor_get_counts(right_motor)) < 190);

            stop_and_reset();
        }
        break;
    case 180:
        //右回転
        ev3_motor_set_power(right_motor, -40);
        ev3_motor_set_power(left_motor, 40);
        while (abs(ev3_motor_get_counts(right_motor)) < 360);

        stop_and_reset();

        while(1);
        break;
    default:
        if (degree > 0)
        {
            //右回転
        }
        else
        {
            //左回転
        }
        printf("****ERROR****\n");
        while (1)
        {
        }
        break;
    }

    stop_and_reset();

    while(1);
}

bool_t detect_color(int mass_num)
{
    if (ev3_color_sensor_get_color(EV3_PORT_2) == map[mass_num])
    {
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////////
//**************   Public   *******************

void init_blocksort_gein()
{
    //いい値リスト
    //1.2:0:0.3
    pid_set_data(&gein, 1.0, 0.001, 0.08);
    pid_set_gain(&gein);
}

//5つ運搬になっているか確認
bool_t is_carried()
{
    int cnt = 0;
    for (int i = 0; i < 5; i++)
    {
        if (block[i].is_carry == true)
        {
            cnt++;
        }
    }
    if (cnt == 5)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//ブロックを運び終了フラグの設定
void set_block_carry(int num, bool_t result)
{
    block[num].is_carry = result;
}
//ブロック位置の設定
void set_block_position(int num, int pos)
{
    block[num].position = pos;
}

//ブロックの運搬フラグを返却
bool_t get_carry(int num)
{
    return block[num].is_carry;
}

//ブロックの位置を返却
int get_position(int num)
{
    return block[num].position;
}

//ブロックの優先度を返却
int get_priority(int num)
{
    return block[num].priority;
}

//ブロック並べ初期化(最初に呼び出す)
void init_blocksort()
{

    //値初期化
    vertical = 0;
    horizon = 0;
    position_now = 10;
    init_map();
    is_finished = false;

    //ブロック並べモード初期化
    bsmode = 0;

    //**色情報入力**
    input_color();

    //**位置情報入力**
    input_position();

    //**優先度情報入力**
    input_priority();

    //**ルート探索**
    search_root();

    //ルート参照
    get_route(12, 6, route);
}

//メイン難所攻略
void blocksort(){

    //****回転****
    //int degree = query_degree(4, 3, 4);
    //printf("degree:%d\n",degree);
    //rotate(degree);
    //escape(degree);

    /****  ルート表示  ****
    printf("Main\n");
    printf("*****ROUTE*****\n");
    for(int j = 0; j < 12; j++){
        printf("%d:",j);
        for(int i = 0; i < 6; i++){
            printf("%d,",route[j][i]);
        }
        printf("\n");
    }
    */

    //ライントレース
    n_line_tracing(50,-1,PID_MIDDLE()-7);
    //printf("B:%d,W:%d\n", PID_LOW(), PID_HIGH());
    /*
    switch(bsmode){
        case 0:
            //  **********************
            //  *  update & rotate   *
            //  ********************** 
            //現在地更新
            position_pass = position_now;
            position_now  = route[vertical][horizon];
            position_next = route[vertical][horizon+1];

             //次のマスが－１だった場合
            if(route[vertical][horizon + 1] == -1){
                 //そこの行は終了なので次の行へ
                 vertical++;
                 //列リセット
                 horizon = 0;
            }

            //****回転****
            int degree = query_degree(position_pass, position_now, position_next);
            rotate(degree);

            bsmode = 1;

            break;

        case 1:
            // *****************************
            // *  色検知  &  ライントレース  *
            // ***************************** 

            //色認識
            //マップ上の色を
            if(detect_color(position_now) == true){
                bsmode = 0;
            }
            break;
    }
    */
}

//終了判定
bool_t block_is_finished()
{
    /*
    if(ルート走り終えた == true){

        //ブロック並べ終了
        return true;
    }else{

        //ブロック並べ未終了
        return false;
    }

    */
    //if(ev3_motor_get_counts(EV3_PORT_C) >= 2000)
    //    return true;

    return false;
}
