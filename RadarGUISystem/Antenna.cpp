#include "Antenna.h"
JammingInfo::JammingInfo(double jnr, double theta, double phi)
	:jNR(jnr), jamTheta(theta), jamPhi(phi)
{

}
Antenna::Antenna(int m, int n, double pi, double ls, double ad, double pd)
	:M(m), N(n), powerSingleArr(pi), loss(ls), amplitudeDiff(ad), phaseDiff(pd)
{
	spaceArr = 0.0075;
}
Array8Antenna::Array8Antenna(int m, int n, double powerSing, double lo,
	double ad, double pd) :Antenna(m, n, powerSing, lo, ad, pd)
{
	subM = 4;
	subN = 2;
	A = eye<imat>(8, 8);
	posMat = ones<imat>(16, 16);
	for (int i = 0; i < 16; i++)
	{
		if (i < 4 || i > 11)
		{
			for (int j = 0; j < 16; j++)
			{
				if (j < 4 || j > 11)
					posMat(i, j) = 0;
			}
		}
	}

}
Array12Antenna::Array12Antenna(int m, int n, double powerSing, double lo,
	double ad, double pd):Antenna(m, n, powerSing, lo, ad, pd)
{
	subM = 4;
	subN = 4;
	A = zeros<imat>(12, 16);
	for (int i = 0; i < 12; i++)
	{
		if (i < 2)
			A(i, i + 1) = 1;
		else if (i < 10)
			A(i, i + 2) = 1;
		else
			A(i, i + 3) = 1;
	}
	posMat = ones<imat>(16, 16);
	for (int i = 0; i < 16; i++)
	{
		if (i < 4 || i > 11)
		{
			for (int j = 0; j < 16; j++)
			{
				if (j < 4 || j > 11)
					posMat(i, j) = 0;
			}
		}
	}
}
Array16Antenna::Array16Antenna(int m, int n, double powerSing, double lo,
	double ad, double pd) :Antenna(m, n, powerSing, lo, ad, pd)
{
	subM = 4;
	subN = 4;
	A = eye<imat>(16, 16);
	posMat = ones<imat>(16, 16);
	for (int i = 0; i < 16; i++)
	{
		if (i < 4)
		{
			for (int j = 0; j < 16; j++)
			{
				if (j < 4-i || j > 11+i)
					posMat(i, j) = 0;
			}
		}
		if (i > 11)
		{
			for (int j = 0; j < 16; j++)
			{
				if (j < i-11 || j > 26-i)
					posMat(i, j) = 0;
			}
		}
	}
	//for (int i = 0; i < 16; i++)
	//{
	//	for (int j = 0; j < 16; j++)
	//		cout << posMat(i, j) << " ";
	//	cout << endl;
	//}
}
//void Antenna::UpdateDirInfo(double theta, double phi)
//{
//	beamDirect[0] = theta;
//	beamDirect[1] = phi;
//}