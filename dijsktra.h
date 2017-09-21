#pragma once

#include "ev3api.h"
//辺構造体
typedef struct _side side;
struct _side {
	//始まり
	int s;
	//終わり
	int e;
	//コスト
	int c;
	struct side* next;
};

/**
* 最短ルートを求める
* routeはn要素数の配列。ゴール頂点以降は各要素に-1代入
* 呼び出し側で要解放
*
* n 頂点数
* sideList 辺リスト
* startIdx スタート頂点
* goalIdx ゴール頂点
* route 最短ルート
* totalCost 最短ルートコスト
* return 0:成功 -1:失敗
**/
int dijsktra(int n, side* sideList, int startIdx, int goalIdx, int **route, int* totalCost);

