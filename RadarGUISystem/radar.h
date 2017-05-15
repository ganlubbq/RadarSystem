#ifndef _RADAR_H
#define _RADAR_H

#include "EchoSignal.h"
#include "head.h"
#include "signalInfo.h"
#include "Antenna.h"
#include "SubarrayReceivedSignal.h"

void PulseCompress(cx_mat &pcEcho, cx_rowvec &sigEcho, cx_rowvec &sigtrans);
void MTD(int cpi, cx_mat &sig);
void DelayCount(int* delayInfo, SignalInfo &sig, double td, int sampleNum, int ringNum);
void UpdateMTInfo(Missile &missileInfo, Target &targetInfo);

#endif