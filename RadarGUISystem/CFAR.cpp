#include "CFAR.h"
#include <iostream>
using namespace std;

CfarInfo::CfarInfo(int x, int y, double z)
	:tatVpos(x), tatRpos(y), tatValue(z)
{

}
CFAR::CFAR()
{
	nProt = 3;//保护单元
	int nRef = 20;
	halfN = nRef / 2;//需要平均的采样点数
	double pfa = 1e-6;
	threshold = (pow(pfa, (-1.0 / double(nRef))) - 1) * double(nRef) * 2;
}
void CFAR::UpdateCfarout(cx_mat &cfartemp)
{
	int cpi = cfartemp.n_rows;
	int sampleNum = cfartemp.n_cols;

	cfarout.clear();
	for (int i = 0; i < cpi; i++)
	{
		for (int j = 0; j < sampleNum; j++)
		{
			double powerTemp1 = 0;
			double powerTemp2 = 0;
			double grMean = 0;
			double outputFinal = 0;
			if (j < nProt + 1)
			{

				for (int k = j + 1 + nProt; k < j + halfN + nProt + 1; k++)
					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
				grMean = powerTemp1 / halfN;
				outputFinal = pow(abs(cfartemp(i, j)), 2);
				double outValue = outputFinal / grMean;
				if (outValue > threshold)
					cfarout.push_back(CfarInfo(i, j, outValue));
			}
			else if (j < nProt + halfN)
			{

				for (int k = 0; k < j - nProt; k++)
					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
				for (int k = j + 1 + nProt; k < j + halfN + nProt + 1; k++)
					powerTemp2 += pow(abs(cfartemp(i, k)), 2);
				grMean = (powerTemp1 + powerTemp2) / (halfN + j - nProt);
				outputFinal = pow(abs(cfartemp(i, j)), 2);
				double outValue = outputFinal / grMean;
				if (outValue > threshold)
					cfarout.push_back(CfarInfo(i, j, outValue));
			}
			else if (j < sampleNum - nProt - halfN)
			{

				for (int k = j - halfN - nProt; k < j - nProt; k++)
					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
				for (int k = j + 1 + nProt; k < j + halfN + nProt + 1; k++)
					powerTemp2 += pow(abs(cfartemp(i, k)), 2);
				grMean = (powerTemp1 + powerTemp2) / (2 * halfN);
				outputFinal = pow(abs(cfartemp(i, j)), 2);
				double outValue = outputFinal / grMean;
				if (outValue > threshold)
					cfarout.push_back(CfarInfo(i, j, outValue));
			}
			else if (j < sampleNum - nProt - 1)
			{

				for (int k = j - halfN - nProt; k < j - nProt; k++)
					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
				for (int k = j + nProt + 1; k < sampleNum; k++)
					powerTemp2 += pow(abs(cfartemp(i, k)), 2);
				grMean = (powerTemp1 + powerTemp2) / (halfN + sampleNum - j - 1 - nProt);
				outputFinal = pow(abs(cfartemp(i, j)), 2);
				double outValue = outputFinal / grMean;
				if (outValue > threshold)
					cfarout.push_back(CfarInfo(i, j, outValue));
			}
			else if (j < sampleNum)
			{

				for (int k = j - halfN - nProt; k < j - nProt; k++)
					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
				grMean = powerTemp1 / halfN;
				outputFinal = pow(abs(cfartemp(i, j)), 2);
				double outValue = outputFinal / grMean;
				if (outValue > threshold)
					cfarout.push_back(CfarInfo(i, j, outValue));
			}
		}
	}
}

//CFAR::CFAR(cx_mat &pcEcho, cx_mat &weights, int cpi, int sampleNum)
//{
//	int nProt = 3;//保护单元
//	int nRef = 20;
//	int halfN = nRef / 2;//需要平均的采样点数
//	double pfa = 1e-6;
//	double threshold = (pow(pfa, (-1.0 / double(nRef))) - 1) * double(nRef) * 2;
////	cout << "threshold: " << threshold << endl;
//	cx_mat cfartemp(cpi, sampleNum);
//	cfartemp = weights.t() * pcEcho;
//	cfarout.clear();
//	for(int i = 0; i < cpi; i++)
//	{
//		for(int j = 0; j < sampleNum; j++)
//		{
//			double powerTemp1 = 0;
//			double powerTemp2 = 0;
//			double grMean = 0;
//			double outputFinal = 0;
//			if(j < nProt+1)
//			{
//				
//				for(int k = j + 1 + nProt; k < j + halfN + nProt + 1; k++)
//					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
//				grMean = powerTemp1 / halfN;
//				outputFinal = pow(abs(cfartemp(i, j)), 2);
//				double outValue = outputFinal / grMean;
//				if(outValue > threshold)
//					cfarout.push_back(CfarInfo(i, j, outValue));
//			}
//			else if(j < nProt + halfN)
//			{
//
//				for(int k = 0; k < j - nProt; k++)
//					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
//				for(int k = j + 1 + nProt; k < j + halfN + nProt + 1; k++)
//					powerTemp2 += pow(abs(cfartemp(i, k)), 2);
//				grMean = (powerTemp1 + powerTemp2) / (halfN + j - nProt);
//				outputFinal = pow(abs(cfartemp(i, j)), 2);
//				double outValue = outputFinal / grMean;
//				if(outValue > threshold)
//					cfarout.push_back(CfarInfo(i, j, outValue));
//			}
//			else if(j < sampleNum - nProt - halfN)
//			{
//				
//				for(int k = j - halfN - nProt; k < j - nProt; k++)
//					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
//				for(int k = j + 1 + nProt; k < j + halfN + nProt + 1; k++)
//					powerTemp2 += pow(abs(cfartemp(i, k)), 2);
//				grMean = (powerTemp1 + powerTemp2) / (2 * halfN);
//				outputFinal = pow(abs(cfartemp(i, j)), 2);
//				double outValue = outputFinal / grMean;
//				if(outValue > threshold)
//					cfarout.push_back(CfarInfo(i, j, outValue));
//			}
//			else if(j < sampleNum - nProt - 1)
//			{
//			
//				for(int k = j - halfN - nProt; k < j - nProt; k++)
//					powerTemp1 += pow(abs(cfartemp(i, k)), 2);
//				for(int k = j + nProt + 1; k < sampleNum; k++)
//					powerTemp2 += pow(abs(cfartemp(i, k)), 2);
//				grMean = (powerTemp1 + powerTemp2) / (halfN + sampleNum - j - 1 - nProt);
//				outputFinal = pow(abs(cfartemp(i, j)), 2);
//				double outValue = outputFinal / grMean;
//				if(outValue > threshold)
//					cfarout.push_back(CfarInfo(i, j, outValue));
//			}
//			else if(j < sampleNum)
//			{
//		
//				for(int k = j - halfN - nProt; k < j - nProt; k++)
//					powerTemp1 += pow(abs(cfartemp(i, k)),2);
//				grMean = powerTemp1 / halfN;
//				outputFinal = pow(abs(cfartemp(i, j)), 2);
//				double outValue = outputFinal / grMean;
//				if(outValue > threshold)
//					cfarout.push_back(CfarInfo(i, j, outValue));
//			}
//		}
//	}
////	cout << "size: " << cfarout.size() << endl;
//}
