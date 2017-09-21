#pragma once

#include "ev3api.h"
#include "blocksort.h"
#include "dijsktra.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////
//Public

//ルート参照
void get_route(int ver, int hor ,int ret[ver][hor]);

//ルート探索
bool_t search_root();

