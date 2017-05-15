#ifndef _TRANSMITSIGNAL_H
#define _TRANSMITSIGNAL_H

#include "head.h"
class TransmitSignal
{
public:
   	cx_rowvec transmitSignal;		//储存信号
	int sampleNum;				//样本数

	TransmitSignal(double pw, double bw, double fs);


};


#endif