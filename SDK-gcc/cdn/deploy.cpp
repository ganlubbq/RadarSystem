#include "deploy.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <set>

//#define _TESTINFILE
//#define _ADJINFO
#define _MAXBANDINFO
using namespace std;

//You need to complete the function
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{
    /***************************************************/
        int basicInfo[3], serverCost[1];
        LineInfo(topo[0], basicInfo);
        LineInfo(topo[2], serverCost); 
        /*malloc linkInfo[][]*/
        int **linkInfo = new int*[basicInfo[1]];
        for(int i=0; i<basicInfo[1]; i++)
            linkInfo[i] = new int[4];
        int linkIndex = basicInfo[1] + 4;
        for(int i = 4; i < linkIndex; i++)
        {
            LineInfo(topo[i], linkInfo[i - 4]);
        }
        /*malloc consumeInfo[][]*/
        int **consumeInfo = new int*[basicInfo[2]];
        for(int i=0; i<basicInfo[2]; i++)
            consumeInfo[i] = new int[3];
        int consumeIndex = linkIndex + 1 + basicInfo[2]; 
        for(int i = linkIndex + 1; i < consumeIndex; i++)
        {
            LineInfo(topo[i], consumeInfo[i - linkIndex - 1]);
        }
    /**************************************************/    
#ifdef _TESTINFILE
        for (int i = 0; i < 3; i++)
            cout << basicInfo[i] << ' ';
        cout << endl;
        cout << serverCost[0] << endl;
        for (int i = 0; i < basicInfo[1]; i++)
        {
            for (int j=0; j<4; j++)
                cout << linkInfo[i][j] << ' ';
            cout << endl;
        }
        for (int i = 0; i < basicInfo[2]; i++)
        {
            for (int j=0; j<3; j++)
                cout << consumeInfo[i][j] << ' ';
            cout << endl;
        }
#endif 
        /***malloc adjacencyMatrix for bandwidth, rentcost, demand*****/
        int **adjBandWidth = new int* [basicInfo[0]];
        int **adjRentCost = new int* [basicInfo[0]];
        int **adjDemand = new int* [basicInfo[0]];
        for(int i = 0; i < basicInfo[0]; i++)
        {
            adjBandWidth[i] = new int[basicInfo[0]];
            adjRentCost[i] = new int[basicInfo[0]];            
        }
        for(int i = 0; i < basicInfo[2]; i++)
            adjDemand[i] = new int[basicInfo[0]];
        /********************initialization******************/
        for(int i = 0; i < basicInfo[0]; i++)
        {
            for(int j = 0; j < basicInfo[0]; j++)
            {
                adjBandWidth[i][j] = 0;
                adjRentCost[i][j] = 0;
            }
        }
        for(int i = 0; i < basicInfo[2]; i++)
        {
            for(int j = 0; j < basicInfo[0]; j++)
                adjDemand[i][j] = 0;
        }
        /**************compute adj matrix********************/
        for(int i = 0; i < basicInfo[1]; i++)
        {
            adjBandWidth[linkInfo[i][0]][linkInfo[i][1]] = linkInfo[i][2];
            adjBandWidth[linkInfo[i][1]][linkInfo[i][0]] = linkInfo[i][2];
            adjRentCost[linkInfo[i][0]][linkInfo[i][1]] = linkInfo[i][3];
            adjRentCost[linkInfo[i][1]][linkInfo[i][0]] = linkInfo[i][3];
        }
        for(int i = 0; i < basicInfo[2]; i++)
        {
            adjDemand[consumeInfo[i][0]][consumeInfo[i][1]] = consumeInfo[i][2];
        }

#ifdef _ADJINFO
        cout << adjBandWidth[linkInfo[basicInfo[1]-1][0]][linkInfo[basicInfo[1]-1][1]] << endl;
        cout << adjRentCost[linkInfo[basicInfo[1]-1][0]][linkInfo[basicInfo[1]-1][1]] << endl;
        cout << adjDemand[consumeInfo[basicInfo[2]-1][0]][consumeInfo[basicInfo[2]-1][1]] << endl;
        
#endif
        /**************compute server num position*********************************/
        int serverNum = int(basicInfo[2] / 2.72);
        int *serverPos = new int[serverNum];
        ComputeServerPosition(consumeInfo, basicInfo[2], serverPos);
#ifdef _MAXBANDINFO
        cout << serverNum << endl;
        for(int i = 0; i < serverNum; i++)
            cout << serverPos[i] << ' ';
        cout << endl;
#endif 
        /*******************************************************/
	char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	write_result(topo_file, filename);
        
        /***************************************************/
        /*********free linkInfo, consumeInfo**************/
        for(int i = 0; i < basicInfo[1]; i++)
            delete linkInfo[i];
        delete linkInfo;
        for(int i = 0; i < basicInfo[2]; i++)
            delete consumeInfo[i];
        delete consumeInfo;
        /************free adj****************/
        for(int i = 0; i < basicInfo[0]; i++)
        {
            delete adjBandWidth[i];
            delete adjRentCost[i];
            delete adjDemand[i];
        }
        delete adjBandWidth;
        delete adjRentCost;
        delete adjDemand;
        /**************************************************/

}

void LineInfo(char *sour, int *numInfo)
{
    int mark = 0, cnt = 0;
    int num[3];
    for(int j = 0; j < 15; j++)
    {
        if(sour[j] >= '0' && sour[j] <= '9')
        {
            num[mark] = sour[j];
            mark++;
        }
        else
        {
            switch(mark)
            {
                case 0:
                   break;
                case 1:
                   numInfo[cnt] = num[0] - '0';
                   cnt++;
                   break;
                case 2:
                   numInfo[cnt] = (num[0] - '0')*10 + (num[1] - '0');
                   cnt++;
                   break;
                case 3:
                   numInfo[cnt] = (num[0] - '0')*100 + (num[1] - '0')*10 + (num[2] - '0');
                   cnt++;
                   break;
            }                    
            mark = 0;
        }
    }
}

void ComputeServerPosition(int **info, int num, int* pos)
{
    /*********max bandwidth node***************************/
    /******malloc node band width*****************/
    int *nodeBandWidth = new int[num];
    for(int i = 0; i < num; i++)
        nodeBandWidth[i] = 0;
    for(int i = 0; i < num; i++)
    {
        nodeBandWidth[info[i][0]] += info[i][2];
        nodeBandWidth[info[i][1]] += info[i][2];
    }
#ifdef _MAXBANDINFO
    for(int i = 0; i < num; i++)
        cout << nodeBandWidth[i] << ' ';
    cout << endl;
#endif
    vector<int> nodetemp;
    for(int i = 0; i < num; i++)
    {
        nodetemp.push_back(nodeBandWidth[i]);
    }
    sort(nodetemp.rbegin(), nodetemp.rend());

    int serverNum = int(num / 2.72);
    int cnt = 0;
    while(cnt < serverNum)
    {
        for(int i = 0; i < num; i++)
        {
            if(nodeBandWidth[i] == nodetemp[cnt])
            {
                pos[cnt] = i;
                nodeBandWidth[i] = 0;
                cnt++;
            }
        }
    }
    delete nodeBandWidth;
}