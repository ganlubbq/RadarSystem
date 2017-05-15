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
	cx_mat receivedSignal;  //���н����ź�
	cx_mat receivedJamming; //�����ź�
	cx_mat receivedNoise;   //�����ź�

	EchoSignal *echo;	//�ز��ź�ָ��
	ArrayReceivedSignal(SignalInfo &signalInfo, Antenna &antenna);
	void UpdateReceivedSignal(SignalInfo &signalInfo, Antenna &antenna,
		Missile &missileInfo, Target &targetInfo);//�������н����ź�
	void Jamming(SignalInfo &signalInfo, Antenna &antenna);//��������ź�
	void Noise(SignalInfo &signalInfo, Antenna &antenna);//��������
	double beamGain(Antenna &antenna, Missile &missileInfo, double lam);//���㲨������
	~ArrayReceivedSignal(){
		delete echo;//�ͷ����һ���ز��ź�
	}

};

#endif