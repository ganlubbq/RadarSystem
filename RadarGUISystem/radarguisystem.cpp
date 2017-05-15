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
	this->setWindowTitle("�״ﵼ��ϵͳ");
	this->showMaximized();
	ui.textEdit->showFullScreen();
	
	clock_t start, finish;
	start = clock();

	/******************��ʼ����Ϣ******************
	*1.������Ϣģ���ʼ��
	*2.Ŀ����Ϣģ���ʼ��
	*		��rcs
	*3.������Ϣģ���ʼ��
	*       ��M,N,����Ԫ���ʣ����,���Ȳ�һ�£���λ��һ��
	*       ���������
	*4.�źŻ�����Ϣ����Ƶ���������������ʣ�ռ�ձȣ�cpi
	*5.���н���ģ���ʼ��
	*		����������
	*		����������
	*6.�������ģ���ʼ��
	*7.CFARģ���ʼ��
	*********************************************/
	Target targetInfo(0.1);
	Missile missileInfo;

	Array12Antenna ann(16, 16, 15, 1, 0, 0);//���߳�ʼ��
	ann.jam.push_back(JammingInfo(0, 30, 30));//������Ϣ

	SignalInfo sinfo(20e9, 25e-6, 10e6, 20e6, 0.2, 16);//�źŻ�����Ϣ

	ArrayReceivedSignal arraysignal(sinfo, ann);//��ʼ�������ź�
	arraysignal.Jamming(sinfo, ann);//��������
	arraysignal.Noise(sinfo, ann);//��������

	SubarrayReceivedSignal subarraySignal;//��������ź�

	CFAR cfarout;//CFAR��ʼ��

	finish = clock();
	connect(this, SIGNAL(sendMessage(QString)), this, SLOT(receiveMessage(QString)));
	sendMessage(tr("��ʼ��..."));
	sendMessage(tr("��ʼ����ʱ�� %1 ms\n").arg(finish - start));
	
	//ui.textEdit->append(tr("��ʼ��..."));
	//ui.textEdit->append(tr("��ʼ����ʱ�� %1 ms\n").arg(finish - start));
	/*************�����ó�ʼ��Ϣ**********************/
	double distance = 2000;//��ʼλ��
	double speed = 0;//��ʼ�ٶ�
	for (int mm = 0; mm < 10; mm++)
	{
		clock_t startmm, finishmm;
		startmm = clock();

		/*****************������Ϣ*************************
		*1.Ŀ��λ�á��ٶȡ����ٶ�
		*2.����λ�á��ٶȡ����ٶȡ���Ŀ��Ϣ
		*3.���߲���ָ��
		*********************************************/
		targetInfo.UpdatePosInfo(0, 0, 0);//����λ��
		targetInfo.UpdateVecInfo(0, 0, 0);//�����ٶ�
		targetInfo.UpdateAccInfo(0, 0, 0);//���¼��ٶ�

		missileInfo.UpdatePosInfo(0, distance, 0);//����λ��
		missileInfo.UpdateVecInfo(0, speed, 0);//�����ٶ�
		missileInfo.UpdateAccInfo(0, 0, 0);//���¼��ٶ�

		UpdateMTInfo(missileInfo, targetInfo);
		/***************�źŴ�������**************************
		*1.�������н����źţ��ź�+����+��������256*totalNum��
		*2.������������źţ�12*totalNum��
		*3.����ز���ȡ����
		*4.����ѹ��+DBF
		*5.MTD����
		*6.CFAR����
		*7.������
		****************************************************/
		arraysignal.UpdateReceivedSignal(sinfo, ann, missileInfo, targetInfo);//�����ź�
		arraysignal.receivedSignal +=
			(arraysignal.receivedJamming + arraysignal.receivedNoise);//�ź�+����+����

		subarraySignal.UpdateSubarraySignal(arraysignal, ann, missileInfo);//������������ź�

		int delayInfo[3];
		DelayCount(delayInfo, sinfo, missileInfo.targetDistance
			, arraysignal.echo->sampleNum, 50);//����ز���ȡ����

		int sizeXCJN = subarraySignal.subarrayCJN.n_rows;
		int delayNum = delayInfo[2] - delayInfo[0] + 1;

		cx_mat pcStap = zeros<cx_mat>(sinfo.cpi, delayNum); //����ѹ�� + ���ݽ�ȡ + DBF
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
			sendMessage(tr("��%1�μ��δ��⵽Ŀ��  ��ʱ %2 ms\n").arg(mm + 1).arg(finishmm - startmm));
			//ui.textEdit->append(tr("��%1�μ��δ��⵽Ŀ��  ��ʱ %2 ms\n").arg(mm + 1).arg(finishmm - startmm));
		}
		else
		{	
			sendMessage(tr("��%1�μ���⵽Ŀ��  ��ʱ %2 ms").arg(mm + 1).arg(finishmm - startmm));
			//ui.textEdit->append(tr("��%1�μ���⵽Ŀ��  ��ʱ %2 ms").arg(mm + 1).arg(finishmm - startmm));
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
			sendMessage(tr("Ŀ���ٶ� %1 m/s").arg(targetV));
			sendMessage(tr("Ŀ����� %1 m\n").arg(targetPos));
			//ui.textEdit->append(tr("Ŀ���ٶ� %1 m/s").arg(targetV));
			//ui.textEdit->append(tr("Ŀ����� %1 m\n").arg(targetPos));
			
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