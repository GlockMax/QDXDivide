// NFont.h: interface for the NFont class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class AFX_EXT_CLASS NFont : public CObject  
{
public:
	void MakeCFont(CFont& ExtFont, const CDC& DC);
	void Set(const CString &Name, int Size);
	CString Write();
	bool Read(const CString &str);
	NFont & operator = (NFont &F);
	int FontSize;
	CString FontName;
	NFont();
	virtual ~NFont();

};

