#include "SubarrayReceivedSignal.h"


void SubarrayReceivedSignal::UpdateSubarraySignal(ArrayReceivedSignal &arrayReceived,
	Antenna &antenna, Missile &missInfo)
{
	int eleM = antenna.M / antenna.subM;//��������Ԫ
	int eleN = antenna.N / antenna.subN;//��������Ԫ

	const double pi = 3.141592653;

	double u0 = cos(missInfo.beamDirect[1] * pi / 180)
		* sin(missInfo.beamDirect[0] * pi / 180);//����ָ�򣬷�λά��������
	double v0 = sin(missInfo.beamDirect[1] * pi / 180);//����ָ�򣬸���ά��������
	/*************************************************/
	cx_vec wEleRow(eleM);//�����ڣ���Ԫ�е���ʸ��
	cx_vec wEleCol(eleN);//�����ڣ���Ԫ�е���ʸ��
	for(int i = 0 ; i < eleM; i++)
	{
		double temp = 2 * pi * antenna.spaceArr * i * u0 / arrayReceived.echo->lamda;
		wEleRow(i) = complex<double>(cos(temp), sin(temp));
	}
	for(int i = 0 ; i < eleN; i++)
	{
		double temp = 2 * pi * antenna.spaceArr * i * v0 / arrayReceived.echo->lamda;
		wEleCol(i) = complex<double>(cos(temp), sin(temp));
	}
	/***************�������λ��ά����Bu*************************/
	cx_mat Bu = zeros<cx_mat>(antenna.M, antenna.subM);
	for (int i = 0; i < antenna.subM; i++)
	{
		for(int j = 0; j < eleM; j++)
			Bu(i*eleM + j, i) = wEleRow(i);
	}
	/****************�����������ά����Bv*************************/
	cx_mat Bv = zeros<cx_mat>(antenna.N, antenna.subN);
	for (int i = 0; i < antenna.subN; i++)
	{
		for(int j = 0; j < eleN; j++)
			Bv(i*eleN + j, i) = wEleCol(i);
	}
	/************************************/
	cx_mat Au = zeros<cx_mat>(antenna.subM, antenna.subM);//����䷽λά����
	cx_mat Av = zeros<cx_mat>(antenna.subN, antenna.subN);//����丩��ά����
	for (int i = 0; i < antenna.subM; i++)
	{
		double temp = 2 * pi * 4 * antenna.spaceArr * i * u0 / arrayReceived.echo->lamda;
		Au(i, i) = complex<double>(cos(temp), sin(temp));
	}
	for (int i = 0; i < antenna.subN; i++)
	{
		double temp = 2 * pi * 4 * antenna.spaceArr * i * v0 / arrayReceived.echo->lamda;
		Av(i, i) = complex<double>(cos(temp), sin(temp));
	}
	/****������ϳ��Ѳ��������ϳɵ���ʸ����ѡ�����B��MN����Sub_M*Sub_N��ά��****/
	cx_mat B = kron(Bu*Au, Bv*Av);

	ivec pos = vectorise(antenna.posMat);
	
	/*********��ȡwin.txt***************/
	ifstream infile;
	infile.open("win.txt");
	vec Win;
	Win.set_size(256);
	for (int i = 0; i < 256; i++)
		infile >> Win(i);
	infile.close();

	/**����Ԫ���������ݽ��������ۺ�****/
	cx_mat cjnTemp = B.t() *
		(repmat(Win % pos, 1, arrayReceived.echo->totalNum)
		% arrayReceived.receivedSignal);
	/******************************************/
	int leng = antenna.subM * antenna.subN;
	vec deltag = ones<vec>(leng)
		+antenna.amplitudeDiff * randn<vec>(leng, 1); //ͨ����һ�·������
	vec deltapsi = antenna.phaseDiff *
		(ones<vec>(leng) -2 * randu<vec>(leng, 1));//ͨ����һ����λ���
	cx_vec tempTr;
	tempTr.set_size(leng);
	for (int i = 0; i < leng; i++)
	{
		double temp = deltapsi(i)* pi / 180;
		tempTr(i) = complex<double>(cos(temp), sin(temp));
	}
	cx_vec deltaTr = deltag % tempTr;//ͨ�����಻һ��
	/*************������಻һ�º�������������*****************************/
	subarrayCJN = antenna.A * (repmat(deltaTr, 1, arrayReceived.echo->totalNum) % cjnTemp);
}