#pragma once

template <typename T> class BPartialArray
{
	friend class RTriRandMesh;
public:

	BPartialArray(size_t iBaseSize = 200)
	{
		BaseSize = iBaseSize;
		LastPartSize = BaseSize;
		LastPos = 0;
		PartsNum = 1;
		Parts[0] = new T[BaseSize];
		Joined = false;
	}

	BPartialArray(const BPartialArray & inArray)
	{
		BaseSize = inArray.BaseSize;
		LastPos = inArray.LastPos;
		PartsNum = inArray.PartsNum;
		Joined = inArray.Joined;
		LastPartSize = BaseSize;
		for (size_t i = 0; i < PartsNum; ++i)
		{
			Parts[i] = new T[LastPartSize];
			memcpy(Parts[i], inArray.Parts[i], LastPartSize * sizeof(T));
			LastPartSize *= 2;
		}
		LastPartSize /= 2;
	}

	~BPartialArray()
	{
		for (size_t i = 0; i < PartsNum; ++i)
			delete[] Parts[i];
	}

	T &Next(void)
	{
		Joined = false;
		++LastPos;
		if (LastPos > LastPartSize)
		{
			// New part needed
			LastPos = 1;
			LastPartSize *= 2;
			Parts[PartsNum] = new T[LastPartSize];
			++PartsNum;
		}
		return Parts[PartsNum - 1][LastPos - 1];
	}

	T &GetAt(size_t Ind) const
	{
		size_t PartsInd = 0;
		size_t CurSize = 0;
		for (size_t PartSize = BaseSize * 2, NextSize = BaseSize; NextSize <= Ind; ++PartsInd, PartSize *= 2)
		{
			CurSize = NextSize;
			NextSize += PartSize;
		}
		return Parts[PartsInd][Ind - CurSize];
	}

	size_t GetSize() const
	{
		size_t Pow2 = 1;
		for (size_t i = 0; i < PartsNum - 1; ++i)
			Pow2 *= 2;
		return BaseSize * (Pow2 - 1) + LastPos;
	}

	size_t GetPartsNum(void) const
	{
		return PartsNum;
	}

	T * GetPart(size_t Ind) const
	{
		return Parts[Ind];
	}

	T *JoinParts()
	{
		Joined = true;
		if (PartsNum == 1)
		{
			if (LastPos > BaseSize / 2)
				return Parts[0];
		}
		size_t WholeSize = GetSize();
		T *OnePiece = new T[WholeSize];
		T *pDest = OnePiece;
		for (size_t i = 0, CurSize = BaseSize; i < PartsNum - 1; ++i, CurSize *= 2)
		{
			memcpy(pDest, Parts[i], CurSize * sizeof(T));
			pDest += CurSize;
			delete[] Parts[i];
		}
		memcpy(pDest, Parts[PartsNum - 1], LastPos * sizeof(T));
		delete[] Parts[PartsNum - 1];
		PartsNum = 1;
		BaseSize = WholeSize;
		LastPos = BaseSize;
		Parts[0] = OnePiece;
		return OnePiece;
	}

	bool IsJoined() const
	{
		return Joined;
	}
protected:
	static const int MaxPartsNum = 32; 
	size_t BaseSize;
	T *Parts[MaxPartsNum];
	size_t PartsNum;
	size_t LastPos;// Real position of last object +1 because size_t is unsigned and we can't start from -1
	size_t LastPartSize;
	bool Joined;
};

