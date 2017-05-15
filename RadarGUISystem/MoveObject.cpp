#include "MoveObject.h"


void Missile::UpdatePosInfo(double coo1, double coo2, double coo3)
{
	coordinate[0] = coo1;
	coordinate[1] = coo2;
	coordinate[2] = coo3;
}

void Missile::UpdateVecInfo(double vel1, double vel2, double vel3)
{
	velocity[0] = vel1;
	velocity[1] = vel2;
	velocity[2] = vel3;
}

void Missile::UpdateAccInfo(double acc1, double acc2, double acc3)
{
	acceleration[0] = acc1;
	acceleration[1] = acc2;
	acceleration[2] = acc3;
}

//void Missile::UpdateDirInfo(Target &tag)
//{
//	targetDistance = norm(coordinate - tag.coordinate);
//	missileDirect[0] = 0;
//	missileDirect[1] = 0;
//}
/***********************************************************/
Target::Target(double rcs):rCS(rcs)
{
	coordinate.set_size(3);
	velocity.set_size(3);
	acceleration.set_size(3);
}

void Target::UpdatePosInfo(double coo1, double coo2, double coo3)
{
	coordinate[0] = coo1;
	coordinate[1] = coo2;
	coordinate[2] = coo3;
}

void Target::UpdateVecInfo(double vel1, double vel2, double vel3)
{
	velocity[0] = vel1;
	velocity[1] = vel2;
	velocity[2] = vel3;
}

void Target::UpdateAccInfo(double acc1, double acc2, double acc3)
{
	acceleration[0] = acc1;
	acceleration[1] = acc2;
	acceleration[2] = acc3;
}