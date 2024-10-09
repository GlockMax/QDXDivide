#include "stdafx.h"
#include "nfavorconcepts.h"

NFavorConcepts::NFavorConcepts(void)
{
}
NFavorConcepts::NFavorConcepts(const NFavorConcepts &In)
{
	operator = (In);
}

NFavorConcepts::~NFavorConcepts(void)
{
}
void NFavorConcepts::AddConcepts(const CString &iConceptNames, const CMapStringToString &Whole, bool Modal)
{
	if (iConceptNames.IsEmpty())
		return;
	CString ConceptNames(iConceptNames);
	while(ConceptNames.GetLength() > 0)
	{
		CString tmp = ConceptNames.SpanExcluding("\n");
		ConceptNames.Delete(0, tmp.GetLength() + 1);
		CString Word;
		if(Whole.Lookup(tmp, Word))
		{
			if(Word.GetLength() > 0)
			{
				int iKey = Word[0];
				if(Word.GetLength() > 1)
					iKey += Word[1] << 8;
				SetAt(iKey, Concept(tmp, Modal));
			}
		}
	}
}
void NFavorConcepts::RemoveNonModal(void)
{
	int Key;
	Concept Value;
	for(POSITION pos = GetStartPosition(); pos; )
	{
		GetNextAssoc(pos, Key, Value);
		if(!Value.IsModal())
			RemoveKey(Key);
	}
}
bool Concept::IsModal(void)
{
	return Modal;
}
const CString & Concept::GetStr(void)
{
	return Str;
}
bool NFavorConcepts::GetStr(const CString &Key, CString &str)
{
	Concept Conc;
	if(Key.GetLength() < 1)
		return false;
	int iKey = Key[0];
	if(Key.GetLength() > 1)
		iKey += Key[1] << 8;
	if(!Lookup(iKey, Conc))
		return false;
	str = Conc.GetStr();
	return true;
}
const NFavorConcepts &NFavorConcepts::operator = (const NFavorConcepts &In)
{
	RemoveAll();
	int Key = '\0';
	Concept Value;
	for(POSITION pos = GetStartPosition(); pos; )
	{
		GetNextAssoc(pos, Key, Value);
		SetAt(Key, Value);
	}
	return *this;
}
