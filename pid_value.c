#include "pid.h"


//Lchでの陰影の最大と最低
static double lch_high;
static double lch_low;

static double middle_val;   //中間
static double gray_val;     //灰色値
static double gray_middle_val;  //灰、白中間



/////////////PRIVATE/////////////////////////////////////////////////


//中間値を取得する
const double GET_MIDDLE_VALUE()
{
          return middle_val;
}



///////////////PUBLIC////////////////////////////////////////////////

const double LCH_HIGH()
{
  return lch_high;
}


const double LCH_LOW()
{
  return lch_low;
}

const double LCH_MIDDLE()
{
  return middle_val;
}

const double LCH_GRAY()
{
  return gray_val;
}

const double LCH_GRAY_MIDDLE()
{
  return gray_middle_val;
}




//PIDで使用する値郡の設定
void pid_init(double high, double low)
{
  lch_high = high;
  lch_low = low;

  //中間値
  middle_val = (lch_high + lch_low) / 2.0;
  gray_val = middle_val + 10;
  gray_middle_val = (gray_val + lch_high) / 2.0;

}


//PIDから操作量を算出する(pid_value.c)
double pid_get_adjust_value(double diff, pid_raw_t* gain)
{
  const double TIME = 0.001;    //計測スパン
  static double Integration = 0.0,
                Comp_total = 0.0;  //総合的な調整値
  static int Past_diff = 0;

  int is_plus;

  double control_p, control_i, control_d, total;



  control_p = diff * gain->p_gain;
  Integration += (diff + (double)Past_diff) / 2.0 * TIME;
  control_i = Integration * gain->i_gain;
  control_d = ((diff - (double)Past_diff) * gain->d_gain) / TIME;

  total = control_p + control_i + control_d;

  Past_diff = diff;

  if(total > 100) total = 100;
  else if(total < -100) total = -100;

//ここに、入れるはずだった。誤差修正慣性プログラム


  return total;
//  return Comp_total;
}



/*
  //調整幅を慣性的に動かす
  //(基準０より絶対値的に実値より計測値が小さい場合はその値になる)

  //計測値が＋か-か
  if(total < 0) is_plus = -1;
  else          is_plus = 1;

  if(Comp_total * is_plus < 0)
    Comp_total = 0;

  //実値を計測値に寄らせる
  Comp_total += 2 * is_plus;


  //実値より計測値が絶対値的に小さい場合また、計測値とは反対の符号になるとき
  if(total * is_plus <= Comp_total * is_plus)
    Comp_total = total; //計測値を実値にする
  */
