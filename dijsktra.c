#include <stdlib.h>
#include <string.h>
#include <limits.h> 
#include "dijsktra.h"

static int getCost(side* sideList, int s, int e);

/* 頂点 */
typedef struct vertile_tag {
	int isFixed;		/* 1:確定、0:未確定 */
	int minCost;		/* 最低必要コスト */
	int prev;		/* 直前頂点 */
}vertex;

int dijsktra(int n, side* sideList, int startIdx, int goalIdx, int **route, int* totalCost) {
	/* メモリ確保 */
	vertex* vertexes = malloc(sizeof(vertex) * n);
	if (vertexes == NULL) {
		return -1;
	}
	memset(vertexes, 0x00, sizeof(vertexes));

	/* 初期化 */
	for (int i = 0; i < n; i++) {
		vertexes[i].minCost = INT_MAX;
		vertexes[i].isFixed = 0;
		vertexes[i].prev = -1;
	}

	/* スタート設定 */
	vertexes[startIdx].minCost = 0;

	while (vertexes[goalIdx].isFixed != 1) {	/* ゴール頂点が確定するまで */
												/* 未確定で、コスト最小頂点を確定済みとする */
		int fix;	/* 確定頂点 */
		int min = INT_MAX;
		for (int i = 0; i < n; i++) {
			if (vertexes[i].isFixed == 0) {
				if (min > vertexes[i].minCost) {
					min = vertexes[i].minCost;
					fix = i;
				}
			}
		}
		vertexes[fix].isFixed = 1;

		/* 確定頂点とつながっている未確定頂点を更新 */
		for (int i = 0; i < n; i++) {
			if (getCost(sideList, fix, i) != INT_MAX
				&& vertexes[i].isFixed == 0) {

				if (vertexes[i].minCost > vertexes[fix].minCost + getCost(sideList, fix, i)) {
					/* 確定頂点を経由するルートの方がよりコストが少ないか */
					vertexes[i].minCost = vertexes[fix].minCost + getCost(sideList, fix, i);
					vertexes[i].prev = fix;
				}
			}
		}
	}

	/* 戻り値作成 */
	int *tmp = NULL;
	tmp = malloc(sizeof(int) * n);
	if (tmp == NULL) {
		return -1;
	}
	int i = 0;
	for (i = 0; i < n; i++) {
		tmp[i] = -1;
	}

	vertex t;
	t = vertexes[goalIdx];
	tmp[0] = goalIdx;
	i = 1;
	while (t.prev != -1) {
		tmp[i++] = t.prev;
		t = vertexes[t.prev];
	}


	*route = malloc(sizeof(int) * n);
	if (tmp == NULL) {
		return -1;
	}
	for (int j = 0; j < n; j++) {
		(*route)[j] = -1;
	}

	int j = 0;
	*totalCost = 0;
	for (int i = n - 1; i >= 0; i--) {
		if (tmp[i] != -1) {
			(*route)[j++] = tmp[i];
		}
	}

	for (int i = 0; i < n - 1; i++) {
		if ((*route)[i + 1] != -1) {
			*totalCost = *totalCost + getCost(sideList, (*route)[i], (*route)[i + 1]);
		}
	}

	free(vertexes);
	free(tmp);
	vertexes = NULL;
	return 0;
}

static int getCost(side* sideList, int s, int e) {
	side* t;
	for (t = &(sideList[0]); t != NULL; t = t->next) {
		if (t->s == s && t->e == e) {
			return t->c;
		}
	}
	return INT_MAX;
}