#include "ArrayReceivedSignal.h"
#include "head.h"

ArrayReceivedSignal::ArrayReceivedSignal(SignalInfo &signalInfo, Antenna &antenna)
{
	echo = new EchoSignal(0, 0, 0, signalInfo);//分配初始回波信号
	int leng = antenna.M * antenna.N;
	int totalNum = signalInfo.cpi *
		int(signalInfo.pulseWidth * signalInfo.freqSample);//总采样点数
	receivedSignal.set_size(leng, totalNum);
	receivedJamming.set_size(leng, totalNum);
	receivedNoise.set_size(leng, totalNum);
}
void ArrayReceivedSignal::UpdateReceivedSignal(
	SignalInfo &signalInfo, Antenna &antenna,
	Missile &missileInfo, Target &targetInfo)
{
	const double pi = 3.141592653;
	const double c = 3.0e8;
	/*****************雷达发射增益*********************/
	double k = sqrt((antenna.powerSingleArr * pow(signalInfo.lamda, 2)* targetInfo.rCS) /
		(64 * pi * antenna.loss)) / pow(missileInfo.targetDistance, 2);
	double G=beamGain(antenna, missileInfo, signalInfo.lamda);
	k *= G;
	/*********************回波信号************************
	**每更新一次释放前一次回波信号
	**每更新一次分配一次回波
	****************************************************/
	double tao = (missileInfo.targetDistance * 2) / c;
	double fd = 2 * missileInfo.targetSpeed / signalInfo.lamda;
	delete echo;
	echo = new EchoSignal(k, tao, fd, signalInfo);
	/****************************************************/
	double u = cos(missileInfo.beamDirect[1] * pi / 180)
		*sin(missileInfo.beamDirect[0] * pi / 180);
	double v = sin(missileInfo.beamDirect[1] * pi / 180);
	cx_vec asu(antenna.M);
	cx_vec asv(antenna.N);
	for(int i = 0 ; i < antenna.M; i++)
	{
		double temp = -2 * pi * antenna.spaceArr * i * u / signalInfo.lamda;
		asu(i) = complex<double>(cos(temp), sin(temp));
	}
	for(int i = 0 ; i < antenna.N; i++)
	{
		double temp = -2 * pi * antenna.spaceArr * i * v / signalInfo.lamda;
		asv(i) = complex<double>(cos(temp), sin(temp));
	}
	cx_vec as = kron(asu, asv);
	

	receivedSignal = as * echo->echoSignal;


}

void ArrayReceivedSignal::Jamming(SignalInfo &signalInfo, Antenna &antenna)
{

	/******************干扰建模***********************************/
	double K = 1.38e-23;
	double T = 290;
	double Fn = 1;
	double pi = 3.141592653;
	double noisePower = Fn * K * T * signalInfo.bandWidth;

	int leng = antenna.M * antenna.N;
	
	receivedJamming.zeros(leng, echo->totalNum);
	for (auto iter = antenna.jam.begin(); iter != antenna.jam.end(); iter++)
	{
		double jammingPower = pow(10, (iter->jNR) / 10) * noisePower;
		double jamPow = sqrt(jammingPower / 2);

		cx_rowvec jamming = jamPow * randn<cx_rowvec>(echo->totalNum);

		double uj = cos((iter->jamPhi) * pi / 180)* sin((iter->jamTheta) * pi / 180);
		double vj = sin((iter->jamPhi) * pi / 180);
		cx_vec aju(antenna.M);
		cx_vec ajv(antenna.N);
		for (int i = 0; i < antenna.M; i++)
		{
			double temp = -2 * pi * antenna.spaceArr * i * uj / signalInfo.lamda;
			aju(i) = complex<double>(cos(temp), sin(temp));
		}
		for (int i = 0; i < antenna.N; i++)
		{
			double temp = -2 * pi * antenna.spaceArr * i * vj / signalInfo.lamda;
			ajv(i) = complex<double>(cos(temp), sin(temp));
		}
		cx_vec aj = kron(aju, ajv);
		receivedJamming += aj * jamming;//用时大户2
	}
}
void ArrayReceivedSignal::Noise(SignalInfo &signalInfo, Antenna &antenna)
{
	double K = 1.38e-23;
	double T = 290;
	double Fn = 1;
	double noisePower = Fn * K * T * signalInfo.bandWidth;
	double noisePowertemp = sqrt(noisePower / 2);
	int leng = antenna.M * antenna.N;
	receivedNoise = noisePowertemp * randn<cx_mat>(leng, echo->totalNum);
}

double ArrayReceivedSignal::beamGain(Antenna &antenna, Missile &missileInfo, double lamda)
{
	const double pi = 3.141592653;
	double thetaTarget = missileInfo.missileDirect[0] * pi/180;
	double phiTarget = missileInfo.missileDirect[1] * pi / 180;
	double thetaBeam = missileInfo.beamDirect[0] * pi / 180;
	double phiBeam = missileInfo.beamDirect[1] * pi / 180;
	double uTar = cos(phiTarget)*sin(thetaTarget);
	double vTar = sin(phiTarget);
	double uBeam = cos(phiBeam)*sin(thetaBeam);
	double vBeam = sin(phiBeam);
	cx_vec auTar(antenna.M);
	cx_vec avTar(antenna.N);
	cx_vec auBeam(antenna.M);
	cx_vec avBeam(antenna.N);
	for(int i = 0; i < antenna.M; i++)
	{
		double auTartemp = -2 * pi * antenna.spaceArr * i * uTar / lamda;
		auTar(i) = complex<double>(cos(auTartemp), sin(auTartemp));
		double auBeamtemp = -2 * pi * antenna.spaceArr * i * uBeam / lamda;
		auBeam(i) = complex<double>(cos(auBeamtemp), sin(auBeamtemp));
	}
	for(int i = 0; i < antenna.N; i++)
	{
		double avTartemp = -2 * pi * antenna.spaceArr * i * vTar / lamda;
		avTar(i) = complex<double>(cos(avTartemp), sin(avTartemp));
		double avBeamtemp = -2 * pi * antenna.spaceArr * i * vBeam / lamda;
		avBeam(i) = complex<double>(cos(avBeamtemp), sin(avBeamtemp));
	}
	cx_vec aTar = kron(auTar, avTar);
	cx_vec aBeam = kron(auBeam, avBeam);

	ivec pos = vectorise(antenna.posMat);
	
	aTar = aTar % pos;
	aBeam = aBeam % pos;

	cx_mat G1 =aBeam.t() * aTar;

	double G = abs(G1(0, 0));

	return G;
}

