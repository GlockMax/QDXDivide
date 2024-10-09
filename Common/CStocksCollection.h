#pragma once
#include "NCTOOL_API.h"
#include "CMapStringToNStock.h"
#include "NCMProject.h"
#include <set>

class GDynamicCadr;

class NCTOOL_API CStocksCollection :
	public CMapStringToNStock
{
public:
	CStocksCollection(void);
	~CStocksCollection(void);

	void DrawStocks(RenderMode GlobRMode, int Type, BOOL CuttingMode, bool DrawSurf, class MTMachineTool& MT);
	void DrawStocksOld(RenderMode GlobRMode, int Type, BOOL CuttingMode, bool DrawSurf);
	void ReCreateLists();
	void CreateStocksRender();
	void CreateModifStocksRender();
	void AddStock(const CString& name, int& ind, class NStock* pStock);
	CString DeleteStockByInd(INT_PTR index);
	void DeleteStocks();
	bool Swap(INT_PTR first, INT_PTR second);
	void CreateBodiesAndRendersLists();
	void ResetBodiesAndRendersLists();
	class NStock* GetStock(int index);
	class NStock* GetStockUnsafe(int index) const;
	const class NStock* GetLastStock();

	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;

	void ApplyStocksState(NCMApplicationType NewState, const std::vector<BStockGrID>& StGroups);
	NCMApplicationType CStocksCollection::GetStocksState(void);

	void ResetStocks();
	void ClearStocksRender();
	void SetFrozen(bool Val);

	MBody* GetVisibleBodiesList(NCMApplicationType State);
	GBodyRender* GetVisibleRendersList();
	GBodyRender* MakeVisibleRendersList();

	const NColor& GetColor(void);
	void SetColor(const NColor & Col);
	void SetColor(COLORREF Col);

	void Serialize(CArchive& ar);
	void PostSerialize();
	bool RenderSect(class MTMachineTool& MT);

	void SetSharpEdges(bool value);
	void SetSharpEdgesDX(bool value);
	void SetWFrame(bool value) { WFrameAllStocks = value; }

	void SetRMode(RenderMode Mode);
	bool FindTempStock(CString &Name, class NStock*& pStock);
	bool HaveTempStock(void);
	void SetMillStockChanged(bool Val);
	void SetTurnStockChanged(bool Val);
	bool IsSharpEdges() const { return DrawSharpEdgesS; }
	bool IsSharpEdgesDX() const { return DrawSharpEdgesDX; }
	unsigned int GetUuid();
	void ChangeVisibleRendersToNULL(bool MakeCopy);
	bool NeedDraw(enum RenderMode GlobRMode, enum RenderMode ExtRMode);
	void SetCollided(bool Val);
	bool IsCollided() const { return Collided; }
protected:
	void DrawStock(RenderMode GlobRMode, class NStock* pStock, int Type, BOOL CuttingMode, bool DrawSurf);	
	void SetStocksColor(const NColor &color);
	void SetStocksTransparency(double value);
	void SetAlpha(double Alpha);
	void ResetRendersLists();
	void ResetRendersLists(NCMApplicationType type);
	void CreateMixedRendersList();
	void ResetBodiesList(NCMApplicationType type);
	void CreateBodiesList(NCMApplicationType type);
	void SyncProperties();
protected:
	bool WFrameAllStocks;
	bool Collided;
	NColor StocksColor;
	bool DrawSharpEdgesDX;
	CCriticalSection CritSection;
};

