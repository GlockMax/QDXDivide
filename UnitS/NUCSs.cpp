#include "stdafx.h"
#include "NCMPROJECT.h"
#include "NUCSs.h"

int NUCSs::MaxUcsInd = 0;

NUCSs::NUCSs()
{
	DrawName = false;
}

NUCSs::NUCSs(const NUCSs &In)
{
	*this = In;
}

NUCSs::~NUCSs()
{
}

const NCBase & NUCSs::operator[] (int i) const
{
	return UCS[i];
}

NCBase & NUCSs::operator[] (int i)
{
	return UCS[i];
}

void NUCSs::SetUsed(int i)
{
	Used.insert(i);
}

void NUCSs::Serialize(CArchive& ar)
{
	if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < 4.9))
	{
		SerializeElements(ar, &DrawName, 1);
		for (int i = 0; i < MAX_UCS; ++i)
		{
			SerializeElements(ar, &UCS[i].AutoName, 1);
		}
	}
	for (int i = 0; i < MAX_UCS; ++i)
	{
		SerializeElements(ar, (NCBaseOld *)(UCS + i), 1);
		SerializeElements(ar, &UCS[i].Name, 1);
	}
}

void NUCSs::ClearNames()
{
	for (int i = 0; i < MAX_UCS; ++i)
		if(UCS[i].IsAutoName())
			UCS[i].SetName(CString(), true);
}


