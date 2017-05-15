#ifndef _SIGNALINFO_H
#define _SIGNALINFO_H
class SignalInfo
{
public:
	double pulseWidth;		//信号脉宽
	double bandWidth;		//信号带宽
	double freqSample;		//采样率
	double pulseDuty;		//脉冲占空比
	int cpi;				//脉冲相干数
	double freqLoad;       //信号载频
	double lamda;
	SignalInfo(double f0, double pw, double bw, double fs,
		double pd, double cp );

};

#endif