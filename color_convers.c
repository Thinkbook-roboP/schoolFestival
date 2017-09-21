#include "color_convers.h"

#define XN          95.045
#define YN          100.0
#define ZN          108.892

static int LCH_R_HIGH = 0;
static int LCH_G_HIGH = 0;
static int LCH_B_HIGH = 0;



//CIE_RGBを求める
lch_raw_t change_rgb(rgb_raw_t* rgb)
{
          lch_raw_t a;

          if((a.l = rgb->r / (double)LCH_R_HIGH) > 1.0) a.l = 1.0;
          if((a.c = rgb->g / (double)LCH_G_HIGH) > 1.0) a.c = 1.0;
          if((a.h = rgb->b / (double)LCH_B_HIGH) > 1.0) a.h = 1.0;

          return a;
}

//

//f(t)を求める関数
static double f_collect(double t)
{
          if(t > pow((6.0 / 29.0), 3))
                    return (pow(t, (1.0 / 3.0)));

          return (1.0 / (3.0 * (pow((29.0 / 6.0), (2.0 * t)))) + (4.0 / 29.0));
}


bool_t to_lab(rgb_raw_t* rgb, lch_raw_t* lab)
{
          lch_raw_t n_rgb, xyz;

          //rgbを0~1に変換する
          n_rgb = change_rgb(rgb);

          //あまりにも実値が小さすぎるため100倍をして整数の値に近くさせる。誤差は計算上切り捨てられてしまう
          xyz.l = (0.4124 * n_rgb.l + 0.3576 * n_rgb.c + 0.1805 * n_rgb.h) * 100;
          xyz.c = (0.2126 * n_rgb.l + 0.7152 * n_rgb.c + 0.0722 * n_rgb.h) * 100;
          xyz.h = (0.0193 * n_rgb.l + 0.1192 * n_rgb.c + 0.9505 * n_rgb.h) * 100;

          lab->l = 116.0 * f_collect(xyz.l / XN) - 16.0;
          lab->c = 500.0 * (f_collect(xyz.l / XN) - f_collect(xyz.c / YN));
          lab->h = 200.0 * (f_collect(xyz.c / YN) - f_collect(xyz.h / ZN));

          return true;
}

///////////////////////////////////////////////////////////////////////////////////
//Public

void lch_init(rgb_raw_t* rgb)
{
          LCH_R_HIGH = rgb->r;
          LCH_G_HIGH = rgb->g;
          LCH_B_HIGH = rgb->b;
}


bool_t to_lch(rgb_raw_t* rgb ,lch_raw_t* lab)
{
          double old_a;

          to_lab(rgb, lab);

          old_a = lab->c;

          lab->c = sqrt(lab->c * lab->c + lab->h * lab->h);
          lab->h = atan(lab->h / old_a);

          return true;
}


