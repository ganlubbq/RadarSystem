#ifndef _MOVEOBJECT_H
#define _MOVEOBJECT_H
#include "head.h"

/************��������************************/
class Missile 
{
public:	
	vec coordinate; //λ����Ϣ
	vec velocity;		//�ٶ���Ϣ
	vec acceleration;	//���ٶ���Ϣ
	/********��Ŀ��Ϣ*************/
	double targetDistance; //Ŀ�����
	double targetSpeed;	//Ŀ���ٶ�
	vec missileDirect;	//��Ŀ����
	vec beamDirect; //����ָ��
	/**************************/
	Missile()
	{
		coordinate.set_size(3);
		velocity.set_size(3);
		acceleration.set_size(3);
		missileDirect.set_size(2);
		beamDirect.set_size(2);
	}
	void UpdatePosInfo(double, double, double);
	void UpdateVecInfo(double, double, double);
	void UpdateAccInfo(double, double, double);
	//void UpdateDirInfo(Target &tag);
};
/************Ŀ�����************************/
class Target
{
public:	
	vec coordinate;
	vec velocity;
	vec acceleration;
	double rCS;
	Target(double);
	void UpdatePosInfo(double, double, double);
	void UpdateVecInfo(double, double, double);
	void UpdateAccInfo(double, double, double);
};
#endif
