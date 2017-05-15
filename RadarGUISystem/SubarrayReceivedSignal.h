#ifndef _SUBARRAYRECEIVEDSIGNAL_H
#define _SUBARRAYRECEIVEDSIGNAL_H
#include "ArrayReceivedSignal.h"
#include "head.h"
class SubarrayReceivedSignal
{
public:
	cx_mat subarrayCJN;
	void UpdateSubarraySignal(ArrayReceivedSignal &arrayReceived,
		Antenna &antenna, Missile &missInfo);
};


#endif