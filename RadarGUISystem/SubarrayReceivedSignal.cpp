#include "SubarrayReceivedSignal.h"


void SubarrayReceivedSignal::UpdateSubarraySignal(ArrayReceivedSignal &arrayReceived,
	Antenna &antenna, Missile &missInfo)
{
	int eleM = antenna.M / antenna.subM;//子阵内阵元
	int eleN = antenna.N / antenna.subN;//子阵内阵元

	const double pi = 3.141592653;

	double u0 = cos(missInfo.beamDirect[1] * pi / 180)
		* sin(missInfo.beamDirect[0] * pi / 180);//波控指向，方位维波束中心
	double v0 = sin(missInfo.beamDirect[1] * pi / 180);//波控指向，俯仰维波束中心
	/*************************************************/
	cx_vec wEleRow(eleM);//子阵内，阵元行导向矢量
	cx_vec wEleCol(eleN);//子阵内，阵元列导向矢量
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
	/***************构造空域方位降维矩阵Bu*************************/
	cx_mat Bu = zeros<cx_mat>(antenna.M, antenna.subM);
	for (int i = 0; i < antenna.subM; i++)
	{
		for(int j = 0; j < eleM; j++)
			Bu(i*eleM + j, i) = wEleRow(i);
	}
	/****************构造空域俯仰降维矩阵Bv*************************/
	cx_mat Bv = zeros<cx_mat>(antenna.N, antenna.subN);
	for (int i = 0; i < antenna.subN; i++)
	{
		for(int j = 0; j < eleN; j++)
			Bv(i*eleN + j, i) = wEleCol(i);
	}
	/************************************/
	cx_mat Au = zeros<cx_mat>(antenna.subM, antenna.subM);//子阵间方位维补相
	cx_mat Av = zeros<cx_mat>(antenna.subN, antenna.subN);//子阵间俯仰维补相
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
	/****对子阵合成已补相的子阵合成导向矢量的选择矩阵B（MN×（Sub_M*Sub_N）维）****/
	cx_mat B = kron(Bu*Au, Bv*Av);

	ivec pos = vectorise(antenna.posMat);
	
	/*********读取win.txt***************/
	ifstream infile;
	infile.open("win.txt");
	vec Win;
	Win.set_size(256);
	for (int i = 0; i < 256; i++)
		infile >> Win(i);
	infile.close();

	/**对阵元级接收数据进行子阵综合****/
	cx_mat cjnTemp = B.t() *
		(repmat(Win % pos, 1, arrayReceived.echo->totalNum)
		% arrayReceived.receivedSignal);
	/******************************************/
	int leng = antenna.subM * antenna.subN;
	vec deltag = ones<vec>(leng)
		+antenna.amplitudeDiff * randn<vec>(leng, 1); //通道不一致幅度误差
	vec deltapsi = antenna.phaseDiff *
		(ones<vec>(leng) -2 * randu<vec>(leng, 1));//通道不一致相位误差
	cx_vec tempTr;
	tempTr.set_size(leng);
	for (int i = 0; i < leng; i++)
	{
		double temp = deltapsi(i)* pi / 180;
		tempTr(i) = complex<double>(cos(temp), sin(temp));
	}
	cx_vec deltaTr = deltag % tempTr;//通道幅相不一致
	/*************加入幅相不一致后的子阵接收数据*****************************/
	subarrayCJN = antenna.A * (repmat(deltaTr, 1, arrayReceived.echo->totalNum) % cjnTemp);
}