#ifndef _ARRAYRECEIVED_H
#define _ARRAYRECEIVED_H

#include "Antenna.h"
#include "MoveObject.h"
#include "EchoSignal.h"
#include "signalInfo.h"
#include "head.h"


class ArrayReceivedSignal
{
public:
	cx_mat receivedSignal;  //阵列接受信号
	cx_mat receivedJamming; //干扰信号
	cx_mat receivedNoise;   //噪声信号

	EchoSignal *echo;	//回波信号指针
	ArrayReceivedSignal(SignalInfo &signalInfo, Antenna &antenna);
	void UpdateReceivedSignal(SignalInfo &signalInfo, Antenna &antenna,
		Missile &missileInfo, Target &targetInfo);//更新阵列接受信号
	void Jamming(SignalInfo &signalInfo, Antenna &antenna);//计算干扰信号
	void Noise(SignalInfo &signalInfo, Antenna &antenna);//计算噪声
	double beamGain(Antenna &antenna, Missile &missileInfo, double lam);//计算波形增益
	~ArrayReceivedSignal(){
		delete echo;//释放最后一个回波信号
	}

};

#endif