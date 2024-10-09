#pragma once

// NExprProperty command target

class NExprProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(NExprProperty)
public:
	NExprProperty(const CString& strName, const COleVariant& varValue, const CString& strDesc);
	virtual ~NExprProperty();

	virtual BOOL OnEdit(LPPOINT lptClick) override;
	virtual BOOL OnEndEdit() override;
	virtual void SetValue(const COleVariant& varValue) override;
	virtual void SetOriginalValue(const COleVariant& varValue) override;
	double GetDValue() const;
protected:
	CString Expression;
	COleVariant dVal;
};


