// NCadr.h: interface for the NCadr class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class NCycle;

class AFX_EXT_CLASS NCadr : public CObject 
{
	friend class NCUnit;
public:
	bool IsTextChanged() const { return TextChanged; }
	void Serialize(CArchive &ar) override;
	const CString& GetOrText() const noexcept;
	const CString& GetReText() const noexcept;
	CString& GetReText2Ch() noexcept;
	CString& GetTextMod() noexcept;
	void CheckReText() noexcept;
	void ClearReText() noexcept;
	void SetOrText(const CString& str);
	void SetReText(const CString& str);
	NCadr(const CString&);
	virtual ~NCadr();
	int GetErrCode() const { return ErrCode; }
	void SetErrCode(int Val) { ErrCode = Val; }
	DECLARE_SERIAL(NCadr);

protected:
	NCadr() noexcept;
	bool TextChanged;
	CString OrText;
	CString ReText;
	NCycle * pCycle;
	int ErrCode;
};
