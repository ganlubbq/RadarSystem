#ifndef _SIGNALINFO_H
#define _SIGNALINFO_H
class SignalInfo
{
public:
	double pulseWidth;		//�ź�����
	double bandWidth;		//�źŴ���
	double freqSample;		//������
	double pulseDuty;		//����ռ�ձ�
	int cpi;				//���������
	double freqLoad;       //�ź���Ƶ
	double lamda;
	SignalInfo(double f0, double pw, double bw, double fs,
		double pd, double cp );

};

#endif