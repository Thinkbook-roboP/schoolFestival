#include "input_code.h"
#include "ev3api.h"

int input_code(void);
void displa(int,int);
int press_button(void);
int error_processing(int,int,int);
void ofuro();



int input_code(){
       //int型5桁で返す
  
    int fcode=12008;//最終的に渡す数字
    int flag=0;//0が押してない　1が上　2が右 3が下　4が左 5が中央
    int plusn=10000;//足す数字　初期は1万
    int error_handling=0;//0問題ない　1繰り上がってしまうため処理を阻止
    int rlcheck=0;//右と左に突き抜けないようにする確認用変数

     ev3_lcd_set_font(EV3_FONT_MEDIUM); //文字サイズ指定ー中
   
         
         ev3_speaker_play_tone(NOTE_C4, 100);

         displa(fcode,plusn);

         while(flag!=5){
            
            flag=press_button();//0が押してない　1が上　2が右 3が下　4が左 5が中央
         

            if(flag==1){
              
                error_handling=error_processing(fcode,plusn,flag);
                
                if(error_handling==0){
                fcode=fcode+plusn;
                ev3_speaker_play_tone(NOTE_C4, 100);
                }
            }else if(flag==2){
                rlcheck=plusn/10;//チェック
                if(rlcheck>0){//0より大きかったら実行
                plusn=plusn/10;
                ev3_speaker_play_tone(NOTE_C4, 100);
                }
            }else if(flag==3){
               
                error_handling=error_processing(fcode,plusn,flag);

                if(error_handling==0){
                fcode=fcode-plusn;
                ev3_speaker_play_tone(NOTE_C4, 100);
                }
            }else if(flag==4){
                rlcheck=plusn*10;//チェック
                if(rlcheck<100000){//100000-10万より小さかったら実行
                plusn=plusn*10;
                ev3_speaker_play_tone(NOTE_C4, 100);
                }
                

            }
            
             
            displa(fcode,plusn);
         }
        
        displa(114514,plusn);
        ofuro();
        return fcode;
}

  int press_button(){
            int pressflag=0;//0が押してない　1が上　2が右 3が下　4が左 5が中央

                if(ev3_button_is_pressed(UP_BUTTON)){//上
                    pressflag=1;
                    tslp_tsk(400);
                }
                if(ev3_button_is_pressed(RIGHT_BUTTON)){//右
                    pressflag=2;
                    tslp_tsk(400);
                }
                if(ev3_button_is_pressed(DOWN_BUTTON)){//下
                    pressflag=3;
                    tslp_tsk(400);
                }
                if(ev3_button_is_pressed(LEFT_BUTTON)){//左
                    pressflag=4;
                    tslp_tsk(400);
                }
                if(ev3_button_is_pressed(ENTER_BUTTON)){//中央
                    pressflag=5;
                    tslp_tsk(400);
                }
            

            
            return pressflag; //0が押してない　1が上　2が右 3が下　4が左 5が中央
        }
 void displa(int plusnum,int ichi){   //数字を画面に表示する関数
          char fncv[32];
          char fnum[10];
          char fi[5];
          if(plusnum==114514){
               sprintf(fncv,"OK!");
         ev3_lcd_draw_string(fncv,10,70);
          }else{
            sprintf(fncv,"first number code");
         ev3_lcd_draw_string(fncv,10,10);
         
         sprintf(fnum,"%05d",plusnum);
         ev3_lcd_draw_string(fnum,10,70);
         
         switch(ichi){
             case 10000:
                 sprintf(fi,"_    ");        //文章
                 ev3_lcd_draw_string(fi,10,50);
                    break;
             case 1000:
                        sprintf(fi," _   ");        //文章
                 ev3_lcd_draw_string(fi,10,50); 
                    break;
             case 100:
              sprintf(fi,"  _  ");        //文章
                 ev3_lcd_draw_string(fi,10,50);
                    break;
             case 10:
              sprintf(fi,"   _ ");        //文章
                 ev3_lcd_draw_string(fi,10,50);
                    break;
             case 1:
            sprintf(fi,"    _");        //文章
                 ev3_lcd_draw_string(fi,10,50);
                    break;
         }

          }

        }
int error_processing(int fer,int keta,int ng){//fer現在の値　keta 現在操作している桁数判定 ng 上1か下3か
    int erpro=0;//0問題ない　1繰り上がってしまうため処理を阻止
    int check=0;
    
    if(keta==10000){//万の位の時のエラー処理
    fer=fer/10000;//万の値を抽出
    if(ng==1){//上が押されたとき
    fer=fer+1;
    if(fer>=10){
        erpro=1;
    }
    }else if(ng==3){//下が押されたとき
    fer=fer-1;
    if(fer<0){
         erpro=1;
    }

    }
    }
    else if(keta==1000){ //千の位の時のエラー処理
    check=fer;     //万の値を消す処理
    check=check/10000*10000;
    fer=fer-check;
    fer=fer/1000;//千の値を抽出
    if(ng==1){//上が押されたとき
    fer=fer+1;
    if(fer>=10){
        erpro=1;
    }
    }else if(ng==3){//下が押されたとき
    fer=fer-1;
    if(fer<0){
         erpro=1;
    }
    }
    }
    else if(keta==100){ //百の位の時のエラー処理
    check=fer;     //万千の値を消す処理
    check=check/1000*1000;
    fer=fer-check;
    fer=fer/100;//千の値を抽出
    if(ng==1){//上が押されたとき
    fer=fer+1;
    if(fer>=10){
        erpro=1;
    }
    }else if(ng==3){//下が押されたとき
    fer=fer-1;
    if(fer<0){
         erpro=1;
    }
    }
    }
    else if(keta==10){//十の位の時のエラー処理
     check=fer;     //万千百の値を消す処理
    check=check/100*100;
    fer=fer-check;
    fer=fer/10;//千の値を抽出

    if(ng==1){//上が押されたとき
    fer=fer+1;
    if(fer>=10){
        erpro=1;
    }
    }else if(ng==3){//下が押されたとき
    fer=fer-1;
    if(fer<0){
         erpro=1;
    }
    }
    }
    else if(keta==1){    //一の位の時のエラー処理
    check=fer;     //万千百十の値を消す処理
    check=check/10*10;
    fer=fer-check;
    

    if(ng==1){//上が押されたとき
    fer=fer+1;
    }if(fer>=10){
        erpro=1;
    }
    else if(ng==3){//下が押されたとき
    fer=fer-1;
    if(fer<0){
         erpro=1;
    }

    }
    
    }    
    return erpro;//0問題ない　1繰り上がってしまうため処理を阻止

    
}
void ofuro(){
     //ev3_speaker_play_tone(NOTE_G4, 100);
     tslp_tsk(100);
     //ev3_speaker_play_tone(NOTE_F4, 100);
     tslp_tsk(100);
     //ev3_speaker_play_tone(NOTE_E4, 200);
      //tslp_tsk(200);
     //ev3_speaker_play_tone(NOTE_G4, 100);
      //tslp_tsk(100);
     //ev3_speaker_play_tone(NOTE_C4, 100);
      //tslp_tsk(100);
}
