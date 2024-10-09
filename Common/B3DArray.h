#pragma once

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

template <typename T> class B3DArray
{
public:
	B3DArray()
	{
		Storage = nullptr;
		ArrayX = nullptr;
		ArrayXY = nullptr;
	}


	virtual ~B3DArray()
	{
		delete[] ArrayX;
		delete[] ArrayXY;
		delete[] Storage;
	}

	virtual void SetSizes(int nX, int nY, int nZ)
	{
		Size = nX * nY * nZ;
		delete[] ArrayX;
		delete[] ArrayXY;
		delete[] Storage;
		Storage = new T[Size];
		ArrayXY = new T *[nX * nY];
		for (int i = 0; i < nX; ++i)
			for (int j = 0; j < nY; ++j)
				ArrayXY[i * nY + j] = &Storage[(i * nY + j) * nZ];
		ArrayX = new T **[nX];
		for (int i = 0; i < nX; ++i)
			ArrayX[i] = &ArrayXY[i * nY];
	}

	T ***GetPtr()
	{
		return ArrayX;
	}

	T &Get(int iX, int iY, int iZ)
	{
		return GetPtr()[iX][iY][iZ];
	}

	T Get(int iX, int iY, int iZ) const
	{
		return ArrayX[iX][iY][iZ];
	}

	T *GetStor() const
	{
		return Storage;
	}

	void SetCommonValue(T Value)
	{
		for (size_t k = 0; k < Size; ++k)
			Storage[k] = Value;
	}

	void Or(const B3DArray &In)
	{
		for (int i = 0; i < Size; ++i)
			Storage[i] |= In.Storage[i];
	}

	auto GetSize()
	{
		return Size;
	}
protected:
	T *Storage;
	T **ArrayXY;
	T ***ArrayX;
	size_t Size;
};


