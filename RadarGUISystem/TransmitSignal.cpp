#include "TransmitSignal.h"
#include <cmath>


TransmitSignal::TransmitSignal(double pw, double bw, double fs)
{
	const double pi = 3.141592653;
	double modulation = bw / pw;
	sampleNum = int(pw * fs);
	transmitSignal.set_size(sampleNum);
	double * trantemp =new  double[sampleNum];
	for(int i = 0; i < sampleNum; i++)
	{
		trantemp[i] = -1 * pw / 2 + pw / sampleNum * i;
		trantemp[i] = pi * modulation * trantemp[i] * trantemp[i];
		transmitSignal(i) = complex<double>(cos(trantemp[i]), sin(trantemp[i]));		
	}
	delete [] trantemp;

}

