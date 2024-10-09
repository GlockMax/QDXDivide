// NMapStringToOb.cpp: implementation of the NMapStringToOb class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NMapStringToOb.h"
#include <utility>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_SERIAL(NMapStringToOb, CMapStringToOb, 0)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NMapStringToOb::NMapStringToOb()
{
	pos = -1;
	ClearSelected();
}

NMapStringToOb::~NMapStringToOb()
{

}

void NMapStringToOb::RemoveAll()
{
	Order.clear();
	clear();
}

void NMapStringToOb::FullRemoveAll()
{
	for(int i = 0; i < GetSize(); ++i)
	{
		CObject *pObj = NULL;
		CString Key;
		GetAt(i, Key, pObj);
		delete pObj;
	}
	RemoveAll();
}

void NMapStringToOb::SetAt(LPCTSTR key, CObject* newValue)
{
	if(find(key) == end())
		Order.push_back(key);
	insert(std::make_pair(CString(key), newValue));
}

void NMapStringToOb::RemoveAt(INT_PTR nIndex)
{
	const CString &key = Order.at(nIndex);
	if(!key.IsEmpty())
	{
		erase(key);
		Order.erase(Order.begin() + nIndex);
	}
}
BOOL NMapStringToOb::RemoveKey(LPCTSTR key)
{
	for(int i = 0;i < Order.size(); ++i)
	{
		if(Order[i] == key)
		{
			Order.erase(Order.begin() + i);
			break;
		}
	}
	return erase(key) != 0;
}
//POSITION NMapStringToOb::GetStartPosition()
//{
//	pos = 0;
//	if(Order.size() == 0)
//		return NULL;
//	return CMapStringToOb::GetStartPosition();// DUMMY POSITION 
//// it is not usable in this class
//}
//void NMapStringToOb::GetNextAssoc(POSITION& rNextPosition, CString& rKey, CObject*& rValue) const
//{
//	if(pos < 0)// GetStartPosition was not called
//	{
//		rNextPosition = NULL;
//		return;
//	}
//	int *ppos = (int *)&pos;
//	if(pos + 1 >= Order.GetSize())
//	{
//		*ppos = (int)Order.GetSize();
//		rKey = Order[Order.GetSize()-1];
//		Lookup(rKey,rValue);
//		rNextPosition = NULL;
//		return;
//	}
//	rKey = Order[pos];
//	Lookup(rKey,rValue);
//	(*ppos)++;
//	rNextPosition = (POSITION)1;
//	return;
//}

NMapStringToOb::NMapStringToOb(INT_PTR nBlockSize)
{
	ClearSelected();
}
CObject*& NMapStringToOb::operator[](LPCTSTR key)
{
	if (find(key) == end())
		SetAt(key, nullptr);
	return std::map<CString, CObject*>::operator [](key);
}
void NMapStringToOb::Serialize(CArchive &ar)
{
	if(ar.IsLoading())
		FullRemoveAll();
	if (ar.IsLoading())
	{
		CStringArray buf;
		buf.Serialize(ar);
		CMapStringToOb buf1;
		buf1.Serialize(ar);
		Order.resize(buf.GetSize());
		for (int i = 0; i < Order.size(); ++i)
			Order[i] = buf[i];
		for (auto i = buf1.GetStartPosition(); i != NULL; )
		{
			CObject* pObj = NULL;
			CString Key;
			buf1.GetNextAssoc(i, Key, pObj);
			insert(std::make_pair(Key, pObj));
		}
	}
	else
	{
		CStringArray buf;
		buf.SetSize(Order.size());
		for (int i = 0; i < Order.size(); ++i)
			buf[i] = Order[i];
		buf.Serialize(ar);
		CMapStringToOb buf1;
		for each (const auto& el in *this)
			buf1[el.first] = el.second;
		buf1.Serialize(ar);
	}
	if(ar.IsLoading())
		ClearSelected();
}

void NMapStringToOb::MoveBefore(const CString& Key, const CString& inKey)
{
//Move inKey element before Key element
	if(Key == inKey)
		return;
	if(find(Key) == end())
		return;
	if(find(inKey) == end())
		return;
	int ini = 0;
	for (; ini < Order.size(); ini++)
		if (Order[ini] == inKey)
			break;
	Order.erase(Order.begin() + ini);
	int i = 0;
	for (; i < Order.size(); i++)
		if (Order[i] == Key)
			break;
	Order.insert(Order.begin() + i, inKey);
}

void * NMapStringToOb::operator [](int index)
{
	if (index < 0 || index >= Order.size())
		return nullptr;

	CString key = Order[index];

	auto it = find(key);
	if(it == end())
		return nullptr;
	
	return it->second;
}

INT_PTR NMapStringToOb::GetSize() const
{
	return Order.size();
}

BOOL NMapStringToOb::GetAt(INT_PTR index, CStringA& rKey, CObject *&rValue) const
{
	if(index < 0 || index >= INT_PTR(Order.size()))
		return FALSE;
	rKey = Order[index];
	auto it = find(rKey);
	rValue = it->second;
	return it != end();
}

CString NMapStringToOb::GetKey(int index) const
{
	if(index < 0 || index >= Order.size())
		return _T("");
	return Order[index];
}

int NMapStringToOb::GetIndex(const CString &str) const
{
	for(int i = 0; i < GetSize(); ++i)
		if(str == Order[i])
			return i;
	return -1;
}
int NMapStringToOb::GetIndexByShortName(const CString &str) const
{
	for(int i = 0; i < GetSize(); ++i)
	{
		CString Name = Order[i];
		int index = Name.ReverseFind( '\\' );
		Name.Delete(0, index + 1);
		CString Buf = str;
		if(Buf.MakeUpper() == Name.MakeUpper())
			return i;
	}
	return -1;
}
int NMapStringToOb::GetIndex(const CObject *pValue) const
{
	for(int i = 0; i < GetSize(); ++i)
	{
		auto it = find(Order[i]);
		if (it != end())
			if (it->second == pValue)
				return i;
	}
	return -1;
}
void NMapStringToOb::ClearSelected(void)
{
	Selected = -1;
}

int NMapStringToOb::GetSelected(void)
{
	return Selected;
}

const CString NMapStringToOb::GetSelectedKey(void)
{
	return GetKey(Selected);
}

void NMapStringToOb::SetSelectedKey(const CString &Key)
{
	Selected = GetIndex(Key);
}

bool NMapStringToOb::IsSelectedKey(const CString &Key)
{
	return (Selected == GetIndex(Key));
}

void NMapStringToOb::CopyFrom(NMapStringToOb* pMap)
{
	RemoveAll();
	if (pMap == nullptr)
		return;
	for (int i = 0; i <  pMap->GetSize(); ++i)
	{
		CString key;
		CObject* pOb;
		pMap->GetAt(i, key, pOb);
		SetAt(key, pOb);
	}
}
