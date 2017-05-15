#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
void LineInfo(char *sour, int *numInfo);

void ComputeServerPosition(int **info, int num, int* pos);

void deploy_server(char * graph[MAX_EDGE_NUM], int edge_num, char * filename);

	

#endif
