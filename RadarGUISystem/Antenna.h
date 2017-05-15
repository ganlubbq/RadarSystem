#ifndef _ANTENNA_H
#define _ANTENNA_H
#include "head.h"

class JammingInfo
{
public:
	double jNR; //强度
	double jamTheta;//theta
	double jamPhi;//phi
	JammingInfo(double jnr, double theta, double phi);
};

class Antenna
{
public:
	double powerSingleArr;	//单阵元发射功率
	double loss;			//传输损耗
	vector<JammingInfo> jam; //干扰信息
	double amplitudeDiff; //幅度不一致
	double phaseDiff;//相位不一致
	int M;		//满阵行数
	int N;		//满阵列数

	imat posMat;	//位置矩阵
	double spaceArr;//阵元间距



	/********子阵参数*************/
	imat A; //子阵转移矩阵
	double Dx;	//
	double Dy;
	int subM;  //子阵个数
	int subN;	//子阵个数
	/*****************************/
	Antenna(int m, int n, double powerSing, double lo,
		double ad, double pd);
	//void UpdateDirInfo(double, double);//更新波束指向
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