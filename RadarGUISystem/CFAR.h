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
	int nProt;//������Ԫ
	int halfN;//��Ҫƽ���Ĳ�������
	CFAR();
	double threshold;
//	CFAR(cx_mat &pcEcho, cx_mat &weights, int cpi, int sampleNum);
	void UpdateCfarout(cx_mat &sing);
};
#endif