#pragma once

class Concept
{
public:
	Concept(void){};
	Concept(const CString &InStr, bool InModal)
	{
		Str = InStr;
		Modal = InModal;
	};
	~Concept(){};
	bool IsModal();
	const CString & GetStr(void);

protected:
	CString Str;
	bool Modal;
};

class AFX_EXT_CLASS NFavorConcepts :
	public CMap<int, int, Concept, Concept&>
{
public:
	NFavorConcepts(void);
	NFavorConcepts(const NFavorConcepts &In);
	const NFavorConcepts &operator = (const NFavorConcepts &In);
	~NFavorConcepts(void);
	void RemoveNonModal(void);
	void AddConcepts(const CString &ConceptNames, const CMapStringToString &Whole, bool Modal);
	bool GetStr(const CString & Key, CString &str);
};
