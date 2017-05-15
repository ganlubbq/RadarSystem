#include "EchoSignal.h"

EchoSignal::EchoSignal(double ga, double tao, double fd, SignalInfo &sig)
{
	lamda = sig.lamda;
	const double pi = 3.141592653;
	cPI = sig.cpi;
	sampleNum = int(sig.pulseWidth * sig.freqSample);//一个脉冲周期的采样点数
	totalNum = sig.cpi * sampleNum;						//总采样点数
	double tp = sig.pulseWidth * sig.pulseDuty;
	double taoMohu = fmod(tao, sig.pulseWidth);
	int delayScale = int(taoMohu * sig.freqSample);//目标距离转化为采样点

	vector<complex<double> > monPulseEcho(sampleNum);//一个脉冲的回波数据

	transSignal = new TransmitSignal(tp, sig.bandWidth, sig.freqSample);//发射信号
	if ((delayScale + int(sig.freqSample * tp)) >= sampleNum)
	{
		for(int i = delayScale; i < sampleNum; i++)
			monPulseEcho[i] = transSignal->transmitSignal[i - delayScale];
	}
	else
	{
		for(int i = 0; i < transSignal->sampleNum; i++)
			monPulseEcho[delayScale + i] = transSignal->transmitSignal[i];
	}
	vector<complex<double> > mulpsignal(totalNum); // 单目标多脉冲信号

	for(int i = 0; i < cPI; i++)
	{
		//采用swelling 1模型
		double rcs_1 = 1.0;

		for(int j =0; j < sampleNum; j++)
			mulpsignal[i*sampleNum + j] = rcs_1 * monPulseEcho[j];
	}
	vector<complex<double> > freqMove;
	
	for(int i = 0; i < totalNum; i++)
	{
		double temp = 2 * pi * fd * i / sig.freqSample;
		freqMove.push_back(complex<double>(cos(temp), sin(temp)));
	}
	echoSignal.reset();
	echoSignal.set_size(totalNum);
	for(int i = 0; i < totalNum; i++)
		echoSignal(i) = (ga * mulpsignal[i] * freqMove[i]);
}