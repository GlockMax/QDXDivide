// NPtrArray.h: interface for the NPtrArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPTRARRAY_H__CB2FD4FC_C390_11D5_B0AD_0050BF4A8273__INCLUDED_)
#define AFX_NPTRARRAY_H__CB2FD4FC_C390_11D5_B0AD_0050BF4A8273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class NPtrArray : public CPtrArray  
{
public:
	void Serialize(CArchive &ar);
	NPtrArray();
	virtual ~NPtrArray();

};

#endif // !defined(AFX_NPTRARRAY_H__CB2FD4FC_C390_11D5_B0AD_0050BF4A8273__INCLUDED_)
