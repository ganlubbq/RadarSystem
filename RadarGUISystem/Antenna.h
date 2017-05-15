#ifndef _ANTENNA_H
#define _ANTENNA_H
#include "head.h"

class JammingInfo
{
public:
	double jNR; //ǿ��
	double jamTheta;//theta
	double jamPhi;//phi
	JammingInfo(double jnr, double theta, double phi);
};

class Antenna
{
public:
	double powerSingleArr;	//����Ԫ���书��
	double loss;			//�������
	vector<JammingInfo> jam; //������Ϣ
	double amplitudeDiff; //���Ȳ�һ��
	double phaseDiff;//��λ��һ��
	int M;		//��������
	int N;		//��������

	imat posMat;	//λ�þ���
	double spaceArr;//��Ԫ���



	/********�������*************/
	imat A; //����ת�ƾ���
	double Dx;	//
	double Dy;
	int subM;  //�������
	int subN;	//�������
	/*****************************/
	Antenna(int m, int n, double powerSing, double lo,
		double ad, double pd);
	//void UpdateDirInfo(double, double);//���²���ָ��
};
class Array8Antenna :public Antenna
{
public:
	Array8Antenna(int m, int n, double powerSing, double lo,
		double ad, double pd);
};

class Array12Antenna :public Antenna
{
public:
	Array12Antenna(int m, int n, double powerSing, double lo,
		double ad, double pd);

};
class Array16Antenna :public Antenna
{
public:
	Array16Antenna(int m, int n, double powerSing, double lo,
		double ad, double pd);

};
#endif