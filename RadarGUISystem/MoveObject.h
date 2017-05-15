#ifndef _MOVEOBJECT_H
#define _MOVEOBJECT_H
#include "head.h"

/************导弹参数************************/
class Missile 
{
public:	
	vec coordinate; //位置信息
	vec velocity;		//速度信息
	vec acceleration;	//加速度信息
	/********弹目信息*************/
	double targetDistance; //目标距离
	double targetSpeed;	//目标速度
	vec missileDirect;	//弹目方向
	vec beamDirect; //波束指向
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
/************目标参数************************/
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
