#include "signalInfo.h"

SignalInfo::SignalInfo(double f0, double pw, double bw, double fs,
	double pd, double cp)
	:freqLoad(f0), pulseWidth(pw), bandWidth(bw),
	freqSample(fs), pulseDuty(pd), cpi(cp)
{
	double c = 3e8;
	lamda = c / freqLoad;
}