// NMapStringToOb.h: interface for the NMapStringToOb class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "NCMB_API.h"
#include <map>
#include <vector>

class NCMB_API NMapStringToOb : public std::map<CString, CObject*>
{
public:
	int GetIndex(const CString &str) const;
	int GetIndexByShortName(const CString &str) const;
	int GetIndex(const CObject *pValue) const;
	CString GetKey(int i) const;
	BOOL GetAt(INT_PTR index, CStringA& rKey, CObject * & rValue)const;
	INT_PTR GetSize() const;
	void MoveBefore(const CString& Key, const CString& inKey);
	CObject*& operator[](LPCTSTR key);
	void* operator[](int index);
	NMapStringToOb(INT_PTR nBlockSize);
//	void GetNextAssoc(POSITION& rNextPosition, CString& rKey, CObject*& rValue) const;
//	POSITION GetStartPosition();
	BOOL RemoveKey(LPCTSTR key);
	void RemoveAt(INT_PTR nIndex);
	virtual void RemoveAll();
	void FullRemoveAll();
	NMapStringToOb();
	virtual ~NMapStringToOb();
	void SetAt(LPCTSTR key, CObject* newValue);
	void Serialize(CArchive& ar);
	void ClearSelected(void);
	int GetSelected(void);
//	void SetSelected(int i);
	const CString GetSelectedKey(void);
	void SetSelectedKey(const CString &Key);
	bool IsSelectedKey(const CString &Key);
	void CopyFrom(NMapStringToOb* pMap);
	void AddObj(const CString& name, int& ind, CObject* pObj);

//	DECLARE_SERIAL(NMapStringToOb)

protected:
	int pos;
	std::vector<CString> Order;
	int Selected;
};


enum VisType
{
	VT_NO,
	VT_BYOBJ
};


class NToolCombined;
class NStock;
class NProgram;

typedef NMapStringToOb BMapStringToNProgram;
typedef NMapStringToOb CMapStringToNTool;
typedef NMapStringToOb BMapStringToNStock;
