#ifndef _ECHOSIGNAL_H
#define _ECHOSIGNAL_H

#include "head.h"
#include "TransmitSignal.h"
#include "signalInfo.h"


using namespace std;


class EchoSignal
{
public:
	cx_rowvec echoSignal;			
	int cPI;						//���������
	TransmitSignal *transSignal;
	int totalNum;					//�ܲ���
	int sampleNum;
	double lamda;
	EchoSignal(double ga, double td, double ts, SignalInfo &sig);
	~EchoSignal()
	{
		delete transSignal;
	}

};

#endif