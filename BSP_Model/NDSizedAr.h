#pragma once

class NDSizedAr
{
protected:
	const static int StdSize = 32;
	double Array[StdSize];
	bool Allocated;
	double *pArray;
public:


	NDSizedAr(int Size)
	{
		if(Size > StdSize)
		{
			pArray = new double[Size];
			Allocated = true;
		}
		else
		{
			pArray = Array;
			Allocated = false;
		}
	}

	~NDSizedAr(void)
	{
		if(Allocated)
			delete [] pArray;
	}

	double & operator [] (int Ind)
	{
		return pArray[Ind];
	}
};
