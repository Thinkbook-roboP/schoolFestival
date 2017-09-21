#include "query_degree.h"


/**********************************
 * 付近マスから回転する角度を返却    *
 * int past    一つ前のマス        *
 * int now     現在のマス          *
 * int future  次の目標マス        *
 * return      角度 -:左  +:右     *
 **********************************/
int query_degree(int past,int now,int future){
    
    
    if(past == future){
        return 180;
    }

    switch(past){

        case 0:
            //過去:0の場合現在:10のみ
            switch(future){
                case 1:
                    //過去:0,現在:10,未来:1
                    return -45;
                    break;
                case 5:
                    return 0;
                    break;
                case 12:
                    return 90;
                    break;

            }
        

         
        case 1:
        //過去

        switch(now){
        //現在
            case 2:

            switch(future){
            //未来
                case 5:
                return 150;
                break;

                case 6:
                return 30;
                break;

                case 3:
                return 0;
                break;
            }
                break;
                            
            case 5:
            switch(future){
            //未来
                case 2:
                return -60;
                break;

                case 10:
                return 90;
                break;

                case 8:
                return 0;
                break;
            }                     
                break;

            case 10:
            switch(future){
            //未来
                case 5:
                return -135;
                break;
                case 12:
                return -45;
                break;
            }
                break;
        }
        case 2:
        //過去
        switch(now){
        //現在

            case 1:
            //現在
            switch(future){
                //未来
                case 5:
                return -150;
                break;
                case 10:
                return -105;
                break;
            }
            break;
           
            case 5:
            //現在
            switch(future){
            //未来

            case 1:
            return 60;
            break;
            case 10:
            return -30;
            break;
            case 8:
            return -120;
            break;
            }
            break;
            
            case 6:
            //現在
            switch(future){
            //未来398
            case 3:
            return -60;
            break;
            case 9:
            return 0;
            break;
            case 8:
            return 120;
            break;
            }
            break;

        }
            break;
        
        case 3:
        //過去
        switch(now){
        //3現在2674
                 
            case 2:
            switch(future){
            //32未来156
            case 1:
            //321
            return 0;
            break;
            case 5:
            //325
            return -30;
            break;
            case 6:
            //326
            return -150;
            break;
            }
            break;

            case 6:
            switch(future){

            //36未来289
            case 2:
            //362 
            return 60;
            break;
            case 8:
            //368
            return 0;
            break;
            case 9:
            //369
            return -120;
            break;
            }
            break;
            case 7:
            switch(future){
            //37未来4 11 9
            case 4:
            //374
            return -60;
            break;
            case 11:
            //3711
            return 30;
            break;
            case 9:
            //379
            return 120;
            break;
            }
            break;
            case 4:
                switch(future){
                    //34未来7 11
                    case 7:
                    //347
                        return 150; 
                    break;
                    case 11:
                    //3411
                        return 105;
                    break;
                }
            break;
        }
            break;
    
        case 4:
        //4過去
        switch(now){
        //4現在3 7 11
            case 3:
            switch(future){
            //43未来267
                case 2:
                //432
                return 0;
                break;
                case 6:
                //436
                return -30;
                break;
                case 7:
                //437
                return -150;
                break;
            }
            break;
            case 7:
            switch(future){
            //47未来3911
            case 3:
            //473
            return 60;
            break;
            case 9:
            //479
            return 0;
            break;
            case 11:
            //4711
            return -90;
            break;
            }
            break;
            case 11:
            switch(future){
            //411未来7 15
            case 7:
            return 135;
            break;
            case 15:
            return 45;
            break;
            }
            break;   
        }               
            break;
        case 5:
        //5過去
        switch(now){
        //5現在1 2 10 8
            case 1:
            switch(future){
             //51未来2 10
            case 2:
            //512
            return 150;
            break;
            case 10:
            //51 10
            return -135;
            break;
            }    
            break;
            case 2:
            switch(future){
            //52未来163
            case 1:
            //521
            return -150;
            break;
            case 6:
            //526
            return 60;
            break;
            case 3:
            //523
            return 30;
            break;
            }
            break;
            case 10:
            switch(future){
            //510未来1 12
            case 1:
            //5 10 1
            return 135;
            break;
            case 12:
            //5 10 12
            return -90;
            break;
            }
            break;
            case 8:
            switch(future){
            //58未来6 13 12
            case 6:
            //586
            return -60;
            break;
            case 13:
            //58 13
            return 30;
            break;
            case 12:
            //58 12
            return 90;
            break;
            }
            break;
        }  
        break;
        case 6:
        //過去
        switch(now){
        //6現在2389
            case 2:
            switch(future){
            //62未来153
            case 1:
            //621
            return -30;
            break;
            case 5:
            //625
            return -60;
            break;
            case 3:
            //623
            return 150;
            break;
            }
            break;
            case 3:
            switch(future){
            //63未来274
            case 2:
            //632
            return -150;
            break;
            case 7:
            //637
            return 60;
            break;
            case 4:
            //634
            return 30;
            break;
            }
            break;
            case 8:
            switch(future){
            //68未来5 12 13
            case 5:
            //685
            return 60;
            break;
            case 12:
            //68 12
            return -30;
            break;
            case 13:
            //68 13
            return -90;
            break;
            }
            break;
            case 9:
            switch(future){
            //69未来14 15 7
            case 14:
            //69 14
            return 90;
            break;
            case 15:
            //69 15
            return 30;
            break;
            case 7:
            //697
            return -60;
            break;
            }
            break;
        }                                               
            break;
        case 7:
        //7過去
        switch(now){
        //7現在349 11
            case 3:
            //73未来246
            switch(future){
            case 2:
            //732
            return -30;
            break; 
            case 4:
            //734
            return 150;
            break;
            case 6:
            //736
            return -60;
            break;
            }
            break; 
            case 4:
            //74未来3 11
            switch(future){
            case 3:
            //743
            return -150;
            break;
            case 11:
            //74 11
            return 135;
            break;
            }
            break;
            case 9:
            //79未来6 14 15
            switch(future){
            case 6:
            //796
            return 60;
            break;
            case 14:
            //79 14
            return -30;
            break;
            case 15:
            //79 15
            return -90;
            break;
            }
            break;
            case 11:
            //7 11未来4 15
            switch(future){
            case 4:
            //7 11 4
            return -135;
            break;
            case 15:
            //7 11 15
            return 90;
            break;
            }
            break;                                                      
        }
            break;
        case 8:
        //8過去
        switch(now){
        //8現在56 12 13
            case 5:
            //85未来1 2 10
            switch(future){
            case 1:
            //851
            return 0;
            break;
            case 2:
            //852
            return 120;
            break;
            case 10:
            //8510
            return -90;
            break;
            }
            break;
            case 6:
            //86未来2 3 9
            switch(future){
            case 2:
            //862
            return -120;
            break;
            case 3:
            //863
            return 0;
            break;
            case 9:
            //869
            return 60;
            break;
            }
            break;
            case 12:
            //8 12未来10 13
            switch(future){
            case 10:
            //8 12 10
            return 90;
            break;
            case 13:
            //8 12 13
            return -120;
            break;
            }
            break;
            case 13:
            //8 13未来12 14
            switch(future){
            case 12:
            //8 13 12
            return 120;
            break;
            case 14:
            //8 13 14
            return -60;
            break;
            }
            break;
        }
            break;
        case 9:
        //9過去
        switch(now){
        //9現在67 14 15
        case 6:
        //96未来238
        switch(future){
        case 2:
        //962
        return 0;
        break;
        case 3:
        //963
        return 120;
        break;
        case 8:
        //968
        return -60;
        break;
        }
        break;
        case 7:
        //97未来34　11
        switch(future){
        case 3:
        //973
        return -120;
        break;
        case 4:
        //974
        return 0;
        break;
        case 11:
        //97 11
        return 90;
        break;
        }
        break;
        case 14:
        //9 14未来13 15
        switch(future){
        case 13:
        //9 14 13
        return 60;
        break;
        case 15:
        //9 14 15
        return -120;
        break;
        }
        break;
        case 15:
        //9 15未来14 11
        switch(future){
        case 14:
        //9 15 14
        return 120;
        break;
        case 11:
        //9 15 11
        return -90;
        break;
        }
        break;
        
    }

        break;
        case 10:
        //10過去
        switch(now){
        //10現在1 5 12
        case 1:
        //10 1未来25
        switch(future){
        case 2:
        //10 1 2
        return 105;
        break;
        case 5:
        //10 1 5
        return 135;
        break;
        }
        break;
        case 5:
        //10 5未来128
        switch(future){
        case 1:
        //10 5 1
        return -90;
        break;
        case 2:
        //10 5 2
        return 30;
        break;
        case 8:
        //10 5 8
        return 90;
        break;
        }
        break;
        case 12:
        //10 12未来8 13
        switch(future){
        case 8:
        //10 12 8
        return -90;
        break;
        case 13:
        //10 12 13
        return -30;
        break;
        }
        break;
    }
            break;
        case 11:
        //11過去
        switch(now){
        //11現在47 15
        case 4:
        //11 4未来37
        switch(future){
         case 3:
        //11 4 3
        return -105;
        break;
        case 7:
        //11 4 7
        return -135;
        break;
        }
        break;
        case 7:
        //11 7未来349
        switch(future){
        case 3:
        //11 7 3
        return -30;
        break;
        case 4:
        //11 7 4
        return 90;
        break;
        case 9:
        //11 7 9
        return -90;
        break;
        }
        break;
        case 15:
        //11 15未来9 14
        switch(future){
        case 9:
        //11 15 9
        return 90;
        break;
        case 14:
        //11 15 14
        return 30;
        break;
        }
        break;
    }
        break;
        case 12:
        //12過去
        switch(now){
        //12現在8 10 13
        case 8:
        //12 8未来56 13
        switch(future){
        case 5:
        //12 8 5
        return -90;
        break;
        case 6:
        //12 8 6
        return 30;
        break;
        case 13:
        //12 8 13
        return 120;
        break;
        }
        break;
        case 10:
        //12 10未来1 5
        switch(future){
        case 1:
        //12 10 1
        return 45;
        break;
        case 5:
        //12 10 5
        return 90;
        break;
        }
        break;
        case 13:
        //12 13未来8 14
        switch(future){
        case 8:
        //12 13 8
        return -120;
        break;
        case 14:
        //12 13 14
        return 0;
        break;
        }
        break;
    }
        break;
        case 13:
        //13過去
        switch(now){
        //13現在8 12 14
        case 8:
        //13 8未来56 12
        switch(future){
        case 5:
        //13 85
        return -30;
        break;
        case 6:
        //13 86
        return 90;
        break;
        case 12:
        //13 8 12
        return -120;
        break;
        }
        break;
        case 12:
        //13 12未来10 8
        switch(future){
        case 10:
        //13 12 10
        return 30;
        break;
        case 8:
        //13 12 8
        return 120;
        break;
        }
        break;
        case 14:
        //13 14未来9 15
        switch(future){
        case 9:
        //13 14 9
        return -60;
        break;
        case 15:
        //13 14 15
        return 0;
        break;
        }
        break;
    }
        break;
        case 14:
        //14過去
        switch(now){
        //14現在9 13 15
        case 9:
        //14 9未来6 7 15
        switch(future){
        case 6:
        //14 9 6
        return -90;
        break;
        case 7:
        //14 97
        return 30;
        break;
        case 15:
        //14 9 15
        return 120;
        break;
        }
        break;
        case 13:
        //14 13未来8 12
        switch(future){
        case 8:
        //14 13 8
        return 60;
        break;
        case 12:
        //14 13 12
        return 0;
        break;
        }
        break;
        case 15:
        //14 15未来9 11
        switch(future){
        case 9:
        //14 15 9
        return -120;
        break;
        case 11:
        //14 15 11
        return -30;
        break;
        }
        break;
    }
        break;
        case 15:
        //15過去
        switch(now){
        //15現在9 11 14
        case 9:
        //15 9未来67 14
        switch(future){
        case 6:
        //15 9 6
        return -30;
        break;
        case 7:
        //15 9 7
        return 90;
        break;
        case 14:
        //15 9 14
        return-120;
        break;
        }
        break;
        case 11:
        //15 11未来74
        switch(future){
        case 7:
        //15 11 7
        return -90;
        break;
        case 4:
        //15 11 4
        return -45;
        break;
        }
        break;
        case 14:
        //15 14未来13 9
        switch(future){
        case 13:
        //15 14 13
        return 0;
        break;
        case 9:
        //15 14 9
        return 120;
        break;
        }
        break;
                }
        break;
        
            }
        }


