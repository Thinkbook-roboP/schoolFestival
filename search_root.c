#include "search_root.h"
//////////////////////////////////////////////////////////////////////////////
//*******************   private   ****************************

//マップの頂点数
#define N 16

//決定ルート
static int route[12][6];
//ルート順行カウント
int linecnt = 0;
//現在地
int position_now = 0;
//計算用位置情報
int b_pos[5];
//五角形のブロック数
int in_cnt = 0;	
//全辺数(有向グラフなので実際の辺×２)
int sideNum = 52;

//マスにブロックがあるか確認する
//あったら場合：ブロック番号
//なかった場合：-1
int judge_block_in_mass(int mass){
    for(int i = 0; i < 5; i++){
        if(mass == b_pos[i]){
            //ブロックがあったら
            return i;
        }
    }
    //なかったら
    return -1;
}

//sideListの初期化
void init_sidelist(side* list){  
	//***  1  ***
	list[0].s = 1; list[0].e = 2; list[0].c = 17;
	list[1].s = 1; list[1].e = 5; list[1].c = 10;
	list[2].s = 1; list[2].e = 10; list[2].c = 14;
	//***  2  ***
	list[3].s = 2; list[3].e = 1; list[3].c = 17;
	list[4].s = 2; list[4].e = 3; list[4].c = 17;
	list[5].s = 2; list[5].e = 5; list[5].c = 10;
	list[6].s = 2; list[6].e = 6; list[6].c = 10;
	//***  3  ***
	list[7].s = 3; list[7].e = 2; list[7].c = 17;
	list[8].s = 3; list[8].e = 4; list[8].c = 17;
	list[9].s = 3; list[9].e = 6; list[9].c = 10;
	list[10].s = 3; list[10].e = 7; list[10].c = 10;
	//***  4  ***
	list[11].s = 4; list[11].e = 3; list[11].c = 17;
	list[12].s = 4; list[12].e = 7; list[12].c = 10;
	list[13].s = 4; list[13].e = 11; list[13].c = 14;
	//***  5  ***
	list[14].s = 5; list[14].e = 1; list[14].c = 10;
	list[15].s = 5; list[15].e = 2; list[15].c = 10;
	list[16].s = 5; list[16].e = 8; list[16].c = 10;
	list[17].s = 5; list[17].e = 10; list[17].c = 10;
	//***  6  ***
	list[18].s = 6; list[18].e = 2; list[18].c = 10;
	list[19].s = 6; list[19].e = 3; list[19].c = 10;
	list[20].s = 6; list[20].e = 8; list[20].c = 10;
	list[21].s = 6; list[21].e = 9; list[21].c = 10;
	//***  7  ***
	list[22].s = 7; list[22].e = 3; list[22].c = 10;
	list[23].s = 7; list[23].e = 4; list[23].c = 10;
	list[24].s = 7; list[24].e = 9; list[24].c = 10;
	list[25].s = 7; list[25].e = 11; list[25].c = 10;
	//***  8  ***
	list[26].s = 8; list[26].e = 5; list[26].c = 10;
	list[27].s = 8; list[27].e = 6; list[27].c = 10;
	list[28].s = 8; list[28].e = 12; list[28].c = 10;
	list[29].s = 8; list[29].e = 13; list[29].c = 10;
	//***  9  ***
	list[30].s = 9; list[30].e = 6; list[30].c = 10;
	list[31].s = 9; list[31].e = 7; list[31].c = 10;
	list[32].s = 9; list[32].e = 14; list[32].c = 10;
	list[33].s = 9; list[33].e = 15; list[33].c = 10;
	//*** 10  ***
	list[34].s = 10; list[34].e = 1; list[34].c = 14;
	list[35].s = 10; list[35].e = 5; list[35].c = 10;
	list[36].s = 10; list[36].e = 12; list[36].c = 10;
	//*** 11  ***
	list[37].s = 11; list[37].e = 4; list[37].c = 14;
	list[38].s = 11; list[38].e = 7; list[38].c = 10;
	list[39].s = 11; list[39].e = 15; list[39].c = 10;
	//*** 12  ***
	list[40].s = 12; list[40].e = 8; list[40].c = 10;
	list[41].s = 12; list[41].e = 10; list[41].c = 10;
	list[42].s = 12; list[42].e = 13; list[42].c = 10;
	//*** 13  ***
	list[43].s = 13; list[43].e = 8; list[43].c = 10;
	list[44].s = 13; list[44].e = 12; list[44].c = 10;
	list[45].s = 13; list[45].e = 14; list[45].c = 10;
	//*** 14  ***
	list[46].s = 14; list[46].e = 9; list[46].c = 10;
	list[47].s = 14; list[47].e = 13; list[47].c = 10;
	list[48].s = 14; list[48].e = 15; list[48].c = 10;
	//*** 15  ***
	list[49].s = 15; list[49].e = 9; list[49].c = 10;
	list[50].s = 15; list[50].e = 11; list[50].c = 10;
    list[51].s = 15; list[51].e = 14; list[51].c = 10;
    
	for (int i = 0; i < sideNum - 1; i++) {
		list[i].next = &(list[i + 1]);
	}
	list[sideNum - 1].next = NULL;
}


//ブロックのある位置に隣接するエッジのコストを爆上げする
//運搬時にブロックのあるマスを通らないようにするため
//引数::list:エッジリスト、block_num:今運搬してるブロック番号

//問題点
// エッジコスト更新が１のマス周辺のみ

void update_edgecost(side* list, int block_num){
    ////printf("update::start\n");
    //ブロックの数
    int num = 5;
    //更新するブロックの位置配列
    //運搬するブロックは更新しないため
    int pos[num-1];
    //位置配列に値代入
    //ブロックの総数だけ繰り返し
    ////printf("target:block[%d]::pos:%d\n",block_num,b_pos[block_num]);
    ////printf("Ignore:");
    //配列カウンタ
    int poscnt =  0;
    for(int i = 0; i < 5; i++){
        //運搬するブロック以外のブロックマス更新
        if(i != block_num){
            pos[poscnt] = b_pos[i];
            ////printf("%d:%d,",poscnt,pos[poscnt]);
            poscnt++;
        }
    }
    ////printf("\n");

    //ブロックの位置に隣接するエッジの探索
    int cnt = 0;
    //ブロックの数だけ繰り返し
    for(int i = 0; i < num-1; i++){
        //listの分だけ繰り返し
        ////printf("****%d****\n",pos[i]);
        while(cnt < sideNum){
            //エッジのスタート、エンドマスとブロックの位置が同じ場合
            if(list[cnt].s == pos[i] || list[cnt].e == pos[i]){
                //エッジコストを爆上げ
                ////printf("s:%d,e:%d\n",list[cnt].s, list[cnt].e);
                list[cnt].c = 999;
            }
            cnt++;
        }
        cnt = 0;
    }

    for (int i = 0; i < sideNum - 1; i++) {
		list[i].next = &(list[i + 1]);
	}
	list[sideNum - 1].next = NULL;
}




//ブロック番号から五角形内へのゴールマスを返却
int decide_goal(int num){
    switch(num){
        case 0:
            //黒
            return 8;
            break;
        case 1:
            //赤
            return 14;
            break;
        case 2:
            //黄
            return 13;
            break;
        case 3:
            //青
            return 9;
            break;
        case 4:
            //緑
            return 6;
            break;
        default:
            //printf("ERROR:Line41\n");
            return -1;
            break;
    }
}

//ブロックの強制移動
void move_force(side* sideList, int *list){
    //ルート(一時保存)
    int *ret = NULL;
    //コスト
    int cost;
    int goal = b_pos[list[0]];
    dijsktra(N, sideList, position_now, goal, &ret, &cost);
    
    //ルート保存
    for(int i = 0; i < 6; i++){
        route[linecnt][i] = ret[i];
    }
    linecnt++;
    position_now = goal;
    
    switch(position_now){
        case 6:
            goal = 3;
            break;
        case 8:
            goal = 5;
            break;
        case 13:
            goal = 12;
            break;
        case 9:
            goal = 7;
            break;
        case 14:
            goal = 15;
            break;
    }

    
    //ルート(一時保存)
    int *tmp = NULL;
    //コスト
    int cost2;
    dijsktra(N, sideList, position_now, goal, &tmp, &cost2);
    
    linecnt++;
    position_now = goal;

    b_pos[0] = position_now;


}

//***  コスト比較  ***
//現在地と各ブロックマスへのコストを比較し最小値になるブロック番号を返却
// side:辺リスト uncarry_num:リスト内の未運搬ブロック数　list:配列
int compare_cost(side* sideList, int uncarry_num, int *list){

    //コスト最小値
    int min = 999999;
    //最小値のブロック番号
    int min_num = -1;
    

    //未運搬数の数だけ繰り返し
    for(int i = 0; i < uncarry_num; i++){
        //目標値をリストから代入
        int goal = b_pos[list[i]];
        //ルート(一時保存)
        int *ret = NULL;
        //コスト
        int cost;
        dijsktra(N, sideList, position_now, goal, &ret, &cost);
        free(ret);
        if(min > cost){
            //最小値更新時
            min = cost;
            min_num = list[i];
        }
    }

    //チェックカウンタ
    int checkcnt = 0;
    //ブロック番号
    int num;
    //目標地点にブロックがあるか確認
    if(num = judge_block_in_mass(decide_goal(min_num)) != -1){
        //目標マスにブロックがあった場合
        checkcnt++;
        //次点の目標地点にブロックあるか+五角形内ブロック数を超えてる
        if(num = judge_block_in_mass(decide_goal(num)) != -1){
            checkcnt++;
            if(num = judge_block_in_mass(decide_goal(num)) != -1){
                checkcnt++;
                if(num = judge_block_in_mass(decide_goal(num)) != -1){
                    //五角形内ブロック存在する数超えているので強制撤去
                    move_force(sideList, list);
                }else{
                    //なかったら
                    //目標地点更新
                    min_num = num;
                }
            }else{
                //なかったら
                //目標地点更新
                min_num = num;
            }
        }else{
            //なかったら
            //目標地点更新
            min_num = num;
        }
    }
    
    //最小値ブロック番号を返却
    return min_num;
}


///////////////////////////////////////////////////////////////////////////////////
//******************   public   *****************

//ルート参照
void get_route(int ver, int hor, int ret[ver][hor]){
    for(int j = 0; j < 12; j++){
        for(int i = 0; i < 6; i++){
            ret[j][i] = route[j][i];
        }
    }
}



//ルート探索
bool_t search_root(){
    
    //現在地初期化(一番左の緑マスからスタート)
    position_now = 10;

    //sideList(エッジ情報構造体)のメモリ確保
    side* sideList = (side*)malloc(sizeof(side) * sideNum);
    
    //sideListの初期化
    init_sidelist(sideList);
    
    //位置情報初期化
    for(int i = 0; i < 5; i++){
        b_pos[i] = get_position(i);
    }
    //printf("Init_position\n");
    //printf("0:%d,1:%d,2:%d,3:%d,4:%d\n",b_pos[0],b_pos[1],b_pos[2],b_pos[3],b_pos[4]);
    //決定ルート初期化
    //攻略順リスト
    //初期は「黒、赤、黄、青、緑」
    int order[5] = {0,1,2,3,4};


    //五角形内のブロックリスト
    int in_penta[5] = {-1,-1,-1,-1,-1};
    //五角形内のカウント
    in_cnt = 0;
    //五角形外のブロックリスト
    int out_penta[5] = {-1,-1,-1,-1,-1};
    //五角形外のカウント
    int out_cnt = 0;

    
    //五角形内にあるブロックを＋１し、内外リストを作成
    for(int i = 0; i < 5; i++){
        switch(get_priority(i)){
            case 0:
                //*****優先度==0の場合*****
                //五角形外リストにブロック番号代入
                out_penta[out_cnt] = i;
                out_cnt++;
                break;
            case 1:
                //*****優先度==1の場合*****
                //五角形内リストに代入
                in_penta[in_cnt] = i;
                in_cnt++;
                break;
        }
    }


    //EV3画面初期化
    ev3_lcd_fill_rect(0, 0, 178, 128, EV3_LCD_WHITE);
    ev3_lcd_set_font(EV3_FONT_SMALL);
    

    /** 
     優先度が同じのが複数あるならコスト比較でどれを攻略するか決める
    **/
	//ルート探索回数カウント
    int bcnt = 0;

    //ターゲットとなるブロック番号を代入
    int target = -1;
    
    //*******   ルート探索モード   ***********
    //*** ０：現 在 地～～ブロック　       ***
    //*** １：ブロック～～ブロック置き場    *** 
    //***************************************
    int bmode = 0;

    //五角形内のブロックを運び終わったか
    bool_t is_carry_penta = false;

    //ルート探索
    //5つ運搬済みになるまで繰り返し
    while(is_carried() != true){

        //スタートに現在地を代入
        int start = position_now;
        //ゴールマス初期化
        int goal = -1;

        
        //仮ターゲットリスト
        int temp_list[5] = {-1};
        //仮ターゲットカウント
        int temp_cnt = 0;

        //
        switch(bmode){
            case 0:
                //ルート探索モードが０の場合
                //ターゲットブロックを決定する
                //printf("*** Move MODE ***\n");


                //五角形内から攻略する
                //フラグを確認
                switch(is_carry_penta){
                    case false:
                        /*****************************
                         * 五角形内ブロック未攻略の場合 *
                        *******************************/

                        //五角形にあるところから調べる
                        //五角形内リストの未運搬の数を調べる
                        for(int i = 0; i < in_cnt; i++){
                            if(get_carry(in_penta[i]) == false){
                                temp_list[temp_cnt] = in_penta[i];
                                temp_cnt++;
                            }
                        }
                        //五角形内リストに未運搬の値が１つのみの場合
                        if(temp_cnt == 1){
                            //五角形最後のブロックなので五角形内ブロック攻略フラグを折る
                            is_carry_penta = true;
                            //**  単品からターゲット決定 **
                            //ターゲットにブロックを設定
                            target = temp_list[0];
                        }else{
                            /****************************************
                            *  五角形内リストに未運搬の値が複数の場合  *
                            ****************************************/
                            //** 複数からターゲット決定 **
                            target = compare_cost(sideList, temp_cnt, temp_list);
                        }
                        break;
                    case true:
                        /*****************************
                         * 五角形内ブロック攻略済みの場合 *
                         ******************************/
                        
                        //五角形外にあるところから調べる
                        //五角形外リストの未運搬の数を調べる
                        for(int i = 0; i < out_cnt; i++){
                            if(get_carry(out_penta[i]) == false){
                                temp_list[temp_cnt] = out_penta[i];
                                temp_cnt++;
                            }
                        }
                        //五角形内リストに未運搬の値が１つのみの場合
                        if(temp_cnt == 1){
                            //**  単品からターゲット決定 **
                            //ターゲットにブロックを設定
                            target = temp_list[0];
                        }else{
                            /****************************************
                            *  五角形外リストに未運搬の値が複数の場合  *
                            *****************************************/
                            //** 複数からターゲット決定 **
                            target = compare_cost(sideList, temp_cnt, temp_list);
                        }
                        break;
                }
                //攻略リストにターゲットを設定
                order[bcnt] = target;
                goal = b_pos[target];
                break;
            case 1:
                //printf("*** Carry MODE ***\n");
                
                //ルート探索モードが１の場合
                //ブロックのあるマスを通らないようにエッジリスト更新
                update_edgecost(sideList, order[bcnt]);
                /*  エッジコスト表示
                for(int i = 0; i < sideNum; i+=4){
                    //printf("Cost:%d,%d,%d,%d\n",sideList[i].c,sideList[i+1].c,sideList[i+2].c,sideList[i+3].c);
                }
                */
                //  ２：ブロックマス～～ポリゴンブロック置き場
                //ターゲットマスを決定する
                goal = decide_goal(order[bcnt]);
                break;
        }

        

        
        //ルート(一時保存)
        int *ret = NULL;
        //コスト
        int totalCost = 0;

        ////printf("start:%d  goal:%d\n",start,goal);
        dijsktra(N, sideList, start, goal, &ret, &totalCost);
            
        //printf("Route:%d,%d,%d,%d,%d,%d\n",ret[0],ret[1],ret[2],ret[3],ret[4],ret[5]);
        //printf("routeCost:%d\n",totalCost);


        //ルート保存
        for(int i = 0; i < 6; i++){
            route[linecnt][i] = ret[i];
        }


        
        free(ret);

        //情報更新
        //ルート配列縦番号
        linecnt++;
        //ゴールした地点を現在地に更新
        position_now = goal;

        if(bmode == 1){
            if(order[bcnt] == 0){
                //printf("*** Block BLACK FINISH ***\n");
            }else if(order[bcnt] == 1){
                //printf("*** BLOCK RED FINISH ***\n");
            }else if(order[bcnt] == 2){
                //printf("*** BLOCK YELLOW FINISH ***\n");
            }else if(order[bcnt] == 3){
                //printf("*** BLOCK BLUE FINISH ***\n");
            }else if(order[bcnt] == 4){
                //printf("*** BLOCK GREEN FINISH ***\n");
            }
        }


        //運搬モード時
        if(bmode == 1){
            //運搬フラグを済みに更新
            set_block_carry(order[bcnt], true); 
            //ブロックの位置情報更新
            b_pos[order[bcnt]] = position_now;
            //sideListを元に戻す
            init_sidelist(sideList);
            //ブロック移動終了カウント＋１
            bcnt++;
        }
        


        //モード切り替え
        if(bmode == 0){
            bmode = 1;
        }else if(bmode == 1){
            bmode = 0;
        }
    }

    //printf("FINISH!!\n");
    //printf("Order:%d,%d,%d,%d,%d\n",order[0],order[1],order[2],order[3],order[4]);


    //******  終了時右端の緑まで移動  *********

    //ルート(一時保存)
    int *ret = NULL;
    //コスト
    int totalCost = 0;
    dijsktra(N, sideList, position_now, 11, &ret, &totalCost);
    //printf("Route:%d,%d,%d,%d,%d,%d\n",ret[0],ret[1],ret[2],ret[3],ret[4],ret[5]);
    //ルート保存
    for(int i = 0; i < 6; i++){
        route[linecnt][i] = ret[i];
    }
    linecnt++;
    free(ret);
    

    //ルート表示
    //printf("*****ROUTE*****\n");
    for(int j = 0; j < linecnt; j++){
        //printf("%d:",j);
        for(int i = 0; i < 6; i++){
            //printf("%d,",route[j][i]);
        }
        //printf("\n");
    }
    free(sideList);
    return true;
}
        

