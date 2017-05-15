#include "radar.h"


void PulseCompress(cx_mat &pcEcho, cx_rowvec &sigEcho, cx_rowvec &sigtrans)
{
	int transNum = sigtrans.n_cols;
	int sampleNum = pcEcho.n_cols;
	int nfft = sampleNum + transNum - 1;


	/*********传播信号扩展到nfft维***************/
	vector<complex<double>> Trans;
	for (int i = 0; i < nfft; i++)
	{
		if (i < transNum)
			Trans.push_back(sigtrans[i]);
		else
			Trans.push_back(complex<double>(0, 0));
	}
	/******对trans做fft并保存在fftTrans中********/
	vector<complex<double>> fftTrans(nfft);
	fftw_plan p = fftw_plan_dft_1d(nfft,
		reinterpret_cast<fftw_complex*>(&Trans[0]),
		reinterpret_cast<fftw_complex*>(&fftTrans[0]),
		FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
	/**********利用ffttrans构造匹配滤波器*********/
	vector<complex<double>> matchFilter;
	for (int i = 0; i < nfft; i++)
		matchFilter.push_back(conj(fftTrans[i]));

	/***********一个cpi内的数据做fft***********/
	vector<complex<double>> saveEcho(nfft);
	vector<complex<double>> fftecho(nfft);
	fftw_plan p1 = fftw_plan_dft_1d(nfft,
		reinterpret_cast<fftw_complex*>(&saveEcho[0]),
		reinterpret_cast<fftw_complex*>(&fftecho[0]),
		FFTW_FORWARD, FFTW_ESTIMATE);
	/***********匹配后的数据做ifft*************/
	vector<complex<double>> pciff(nfft);
	vector<complex<double>> pc(nfft);
	fftw_plan p2 = fftw_plan_dft_1d(nfft,
		reinterpret_cast<fftw_complex*>(&pciff[0]),
		reinterpret_cast<fftw_complex*>(&pc[0]),
		FFTW_BACKWARD, FFTW_ESTIMATE);

	for (int i = 0; i != pcEcho.n_rows; i++)
	{
		for (int j = 0; j < sampleNum; j++)
			saveEcho[j] = sigEcho[i*sampleNum + j];

		/************************************/
		fftw_execute(p1);

		for (int j = 0; j < nfft; j++)
			pciff[j] = (fftecho[j] * matchFilter[j]);

		fftw_execute(p2);

		/************************************/
		for (int j = 0; j < sampleNum; j++)
			pcEcho(i, j) = pc[j + 1] / double(sampleNum);

	}
	fftw_destroy_plan(p1);
	fftw_destroy_plan(p2);

}


void MTD(int cpi, cx_mat &dataMTD)
{
	const double pi = 3.141592653;
	cx_mat weights(cpi, cpi);
	for (int i = 0; i < cpi; i++)
	{
		for (int j = 0; j < cpi; j++)
		{
			double st = 2 * pi * j * i / cpi;
			weights(i, j) = complex<double>(cos(st), sin(st));
		}
	}
	dataMTD = weights.t() * dataMTD;
}

//void MTD(SignalInfo &sig, cx_mat &weights, Antenna &ann, SubarrayReceivedSignal &subSig)
//{
//	double pi = 3.141592653;
//	double u0 = cos(ann.beamDirect[1] * pi / 180) * sin(ann.beamDirect[0] * pi / 180);
//	double v0 = sin(ann.beamDirect[1] * pi / 180);
//	cx_vec su0(subSig.subM);
//	cx_vec sv0(subSig.subN);
//	for (int i = 0; i < subSig.subM; i++)
//	{
//		double temp = -2 * pi * subSig.Dx * i * u0 / sig.lamda;
//		su0(i) = complex<double>(cos(temp), sin(temp));
//	}
//	for (int i = 0; i < subSig.subN; i++)
//	{
//		double temp = -2 * pi * subSig.Dy * i * v0 / sig.lamda;
//		sv0(i) = complex<double>(cos(temp), sin(temp));
//	}
//
//	cx_mat st0(sig.cpi, sig.cpi);
//	for (int i = 0; i < sig.cpi; i++)
//	{
//		for (int j = 0; j < sig.cpi; j++)
//		{
//			double temp = 2 * pi  * i * j / sig.cpi;
//			st0(i, j) = complex<double>(cos(temp), sin(temp));
//		}
//	}
//	weights = kron(st0, ann.A * kron(su0, sv0));
//	/**********************************/
//	ofstream out("out.txt");
//	if (out.is_open())
//	{
//		for (int i = 0; i != weights.n_rows; i++)
//		{
//			out << i << ". ";
//			for (int j = 0; j != weights.n_cols; j++)
//				out << weights(i, j) << " ";
//
//			out << endl;
//
//		}
//		out.close();
//	}
//	/*********************************************/
//
//}

void DelayCount(int* delayInfo, SignalInfo &sig, double td, int sampleNum, int ringNum)
{
	double c = 3e8;
	
	//int samRingNum = ringNum * 2;
	double tao = 2 * td / c;
	double taoMohu = fmod(tao, sig.pulseWidth);
	delayInfo[1] = int(sig.freqSample * taoMohu);
	if (delayInfo[1] <= ringNum)
	{
		delayInfo[0] = 1;
		delayInfo[2] = delayInfo[1] + ringNum - 1;
	}
	else if (delayInfo[1] >= sampleNum - ringNum + 1)
	{
		delayInfo[0] = delayInfo[1] - ringNum;
		delayInfo[2] = sampleNum;
	}
	else
	{
		delayInfo[0] = delayInfo[1] - ringNum;
		delayInfo[2] = delayInfo[1] + ringNum - 1;
	}

}

void UpdateMTInfo(Missile &missileInfo, Target &targetInfo)
{
	vec cooediff = missileInfo.coordinate - targetInfo.coordinate;
	vec velodiff = missileInfo.velocity - targetInfo.velocity;
	missileInfo.targetDistance = norm(cooediff);
	missileInfo.missileDirect[0] = 0;
	missileInfo.missileDirect[1] = 0;
	missileInfo.beamDirect[0] = 0;
	missileInfo.beamDirect[1] = 0;
	mat tsTemp = velodiff.t() * cooediff;
	missileInfo.targetSpeed = tsTemp(0, 0) / missileInfo.targetDistance;
}

