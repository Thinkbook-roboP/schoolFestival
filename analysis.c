#include "analysis.h"

///////////////////////////////////////////////////////////////////////////////////
//******  Private  *******

//各色ブロックの初期位置
static int p_black = -1;
static int p_red = -1;
static int p_yellow = -1;
static int p_blue = -1;

///////////////////////////////////////////////////////////////////////////////////
//******  Public  *******
//黒ブロックの位置を返す
int get_position_black(){
    return p_black;
}
//赤ブロックの位置を返す
int get_position_red(){
    return p_red;
}
//黄ブロックの位置を返す
int get_position_yellow(){
    return p_yellow;
}
//青ブロックの位置を返す
int get_position_blue(){
    return p_blue;
}


//コード解析
bool_t analysis(int code){

    
    //黒ブロック位置決定
    p_black = (code / (11*11*11)) + 1;
    code = code - (p_black - 1)*(11*11*11);
    //赤ブロック位置決定
    p_red = (code / (11*11)) + 1;
    code = code - (p_red - 1)*(11*11);
    //黄ブロック位置決定
    p_yellow = (code / 11) + 1;
    code = code - (p_yellow - 1) * 11;
    //青ブロック位置決定
    p_blue = (code / 1) + 1;

    //*** 位置補正 ***
    //赤
    p_red += 1;
    if(p_red >= 6) p_red += 2;
    if(p_red >= 14) p_red += 1;

    //黄
    if(p_yellow >= 3) p_yellow += 1;
    if(p_yellow >= 5) p_yellow += 1;
    if(p_yellow >= 13) p_yellow += 1;

    //青
    if(p_blue >= 2) p_blue += 1;
    if(p_blue >= 4) p_blue += 1;
    if(p_blue >= 9) p_blue += 1;
    if(p_blue >= 12) p_blue += 1;

    
    return true;
}


