#ifndef _TRANSMITSIGNAL_H
#define _TRANSMITSIGNAL_H

#include "head.h"
class TransmitSignal
{
public:
   	cx_rowvec transmitSignal;		//�����ź�
	int sampleNum;				//������

	TransmitSignal(double pw, double bw, double fs);


};


#endif