#pragma once
#include "NStockOrigR.h"
class AFX_EXT_CLASS NStockBSP :
	public NStockOrigR
{
public:
	NStockBSP();
	virtual ~NStockBSP();
	DECLARE_SERIAL(NStockBSP)

public:
	StockCrType GetType() const override;
	bool CreateBody(void) override;
	bool LoadPacked(const CString& SourceName, BYTE*& pBuffer, unsigned long& UnpackedLength);
	void CreateEx(const CString &name); 
	void ReadGabarOnly(const CString &name);
	virtual void CreateCopyTo(class NStock* &stock) const override;
	const BMatr& GetMatr() const override;
	void Transformm(const class BMatr &M) override;
	void Serialize(CArchive &ar) override;
	void Recalc();
	const BPoint & GetBase(void) const { return BaseCenter; }
	bool GetInvert() const { return Invert; }
	void MoveBase(double x, double y, double z, bool Invert);
	void DrawGabar(void) const override;
	BBox GetGabar(void) const override;
	const MBody* GetOrigBody() const;
	bool CreateDXBodyFrBSPRend();
protected:
	void Create(const CString &iFileName, class MQuadTree *pTree, class MQuadTree *pTreeRot = nullptr);
	void Create();
protected:
	int BSPTNumOrig;
	int BSPTNumRotOrig;
	BPoint BaseCenter; // for a turn stock only
	bool Invert; // for a turn stock only
};

