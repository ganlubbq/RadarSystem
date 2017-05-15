#ifndef _CFAR_H
#define _CFAR_H
#include "head.h"

class CfarInfo
{
public:
	int tatVpos;
	int tatRpos;
	double tatValue;
	CfarInfo(int x, int y, double z);
};

class CFAR
{
public:
	vector<CfarInfo> cfarout;
	int nProt;//保护单元
	int halfN;//需要平均的采样点数
	CFAR();
	double threshold;
//	CFAR(cx_mat &pcEcho, cx_mat &weights, int cpi, int sampleNum);
	void UpdateCfarout(cx_mat &sing);
};
#endif