#include "radarguisystem.h"
#pragma execution_character_set("utf-8")
#include "head.h"
#include <ctime>
#include "radar.h"
#include "Antenna.h"
#include "ArrayReceivedSignal.h"
#include "signalInfo.h"
#include "CFAR.h"
#include "SubarrayReceivedSignal.h"


RadarGUISystem::RadarGUISystem(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("雷达导引系统");
	this->showMaximized();
	ui.textEdit->showFullScreen();
	
	clock_t start, finish;
	start = clock();

	/******************初始化信息******************
	*1.导弹信息模块初始化
	*2.目标信息模块初始化
	*		，rcs
	*3.天线信息模块初始化
	*       ，M,N,单阵元功率，损耗,幅度不一致，相位不一致
	*       ，加入干扰
	*4.信号基本信息，载频、脉宽，带宽，采样率，占空比，cpi
	*5.阵列接收模块初始化
	*		，产生干扰
	*		，产生噪声
	*6.子阵接收模块初始化
	*7.CFAR模块初始化
	*********************************************/
	Target targetInfo(0.1);
	Missile missileInfo;

	Array12Antenna ann(16, 16, 15, 1, 0, 0);//天线初始化
	ann.jam.push_back(JammingInfo(0, 30, 30));//干扰信息

	SignalInfo sinfo(20e9, 25e-6, 10e6, 20e6, 0.2, 16);//信号基本信息

	ArrayReceivedSignal arraysignal(sinfo, ann);//初始化阵列信号
	arraysignal.Jamming(sinfo, ann);//产生干扰
	arraysignal.Noise(sinfo, ann);//产生噪声

	SubarrayReceivedSignal subarraySignal;//子阵接收信号

	CFAR cfarout;//CFAR初始化

	finish = clock();
	connect(this, SIGNAL(sendMessage(QString)), this, SLOT(receiveMessage(QString)));
	sendMessage(tr("初始化..."));
	sendMessage(tr("初始化用时： %1 ms\n").arg(finish - start));
	
	//ui.textEdit->append(tr("初始化..."));
	//ui.textEdit->append(tr("初始化用时： %1 ms\n").arg(finish - start));
	/*************测试用初始信息**********************/
	double distance = 2000;//初始位置
	double speed = 0;//初始速度
	for (int mm = 0; mm < 10; mm++)
	{
		clock_t startmm, finishmm;
		startmm = clock();

		/*****************更新信息*************************
		*1.目标位置、速度、加速度
		*2.导弹位置、速度、加速度、弹目信息
		*3.天线波速指向
		*********************************************/
		targetInfo.UpdatePosInfo(0, 0, 0);//更新位置
		targetInfo.UpdateVecInfo(0, 0, 0);//更新速度
		targetInfo.UpdateAccInfo(0, 0, 0);//更新加速度

		missileInfo.UpdatePosInfo(0, distance, 0);//更新位置
		missileInfo.UpdateVecInfo(0, speed, 0);//更新速度
		missileInfo.UpdateAccInfo(0, 0, 0);//更新加速度

		UpdateMTInfo(missileInfo, targetInfo);
		/***************信号处理流程**************************
		*1.产生阵列接受信号（信号+干扰+噪声）（256*totalNum）
		*2.产生子阵接收信号（12*totalNum）
		*3.计算回波截取区间
		*4.脉冲压缩+DBF
		*5.MTD处理
		*6.CFAR处理
		*7.结果输出
		****************************************************/
		arraysignal.UpdateReceivedSignal(sinfo, ann, missileInfo, targetInfo);//更新信号
		arraysignal.receivedSignal +=
			(arraysignal.receivedJamming + arraysignal.receivedNoise);//信号+干扰+噪声

		subarraySignal.UpdateSubarraySignal(arraysignal, ann, missileInfo);//更新子阵接收信号

		int delayInfo[3];
		DelayCount(delayInfo, sinfo, missileInfo.targetDistance
			, arraysignal.echo->sampleNum, 50);//计算回波截取区间

		int sizeXCJN = subarraySignal.subarrayCJN.n_rows;
		int delayNum = delayInfo[2] - delayInfo[0] + 1;

		cx_mat pcStap = zeros<cx_mat>(sinfo.cpi, delayNum); //脉冲压缩 + 数据截取 + DBF
		for (int i = 0; i < sizeXCJN; i++)
		{
			cx_mat temp(sinfo.cpi, arraysignal.echo->sampleNum);
			PulseCompress(temp, conv_to<cx_rowvec>::from(subarraySignal.subarrayCJN.row(i)),
				arraysignal.echo->transSignal->transmitSignal);
			pcStap += temp.cols(delayInfo[0] - 1, delayInfo[2] - 1);
		}

		MTD(sinfo.cpi, pcStap);//MTD

		cfarout.UpdateCfarout(pcStap);//CFAR
		finishmm = clock();
		if (cfarout.cfarout.size() == 0)
		{
			sendMessage(tr("第%1次检测未检测到目标  用时 %2 ms\n").arg(mm + 1).arg(finishmm - startmm));
			//ui.textEdit->append(tr("第%1次检测未检测到目标  用时 %2 ms\n").arg(mm + 1).arg(finishmm - startmm));
		}
		else
		{	
			sendMessage(tr("第%1次检测检测到目标  用时 %2 ms").arg(mm + 1).arg(finishmm - startmm));
			//ui.textEdit->append(tr("第%1次检测检测到目标  用时 %2 ms").arg(mm + 1).arg(finishmm - startmm));
			double maxXvalue = 0;
			double maxYvalue = 0;
			double maxvalue = 0;
			for (auto it = cfarout.cfarout.begin(); it != cfarout.cfarout.end(); it++)
			{
				maxXvalue += (it->tatValue) * (it->tatVpos);
				maxYvalue += (it->tatValue) * (it->tatRpos + delayInfo[0]);
				maxvalue += it->tatValue;
			}
			double maxx = maxXvalue / maxvalue;
			double maxy = maxYvalue / maxvalue;
			double targetV = maxx / sinfo.cpi / sinfo.pulseWidth* sinfo.lamda / 2;
			double targetPos = maxy / sinfo.freqSample * datum::c_0 / 2;
			sendMessage(tr("目标速度 %1 m/s").arg(targetV));
			sendMessage(tr("目标距离 %1 m\n").arg(targetPos));
			//ui.textEdit->append(tr("目标速度 %1 m/s").arg(targetV));
			//ui.textEdit->append(tr("目标距离 %1 m\n").arg(targetPos));
			
		}

		distance -= 100;
		speed += 18.75;

	}



}

RadarGUISystem::~RadarGUISystem()
{

}

void RadarGUISystem::receiveMessage(QString msg)
{
	
	ui.textEdit->append(msg);
	qApp->processEvents();
}