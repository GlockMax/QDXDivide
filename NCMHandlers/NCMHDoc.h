
// NCMDoc.h : interface of the CNCMDoc class
//


#pragma once
#include "CMapStringToNStock.h"
#include "CStocksCollection.h"
#include "RLines.h"
#include "BImageRGB.h"


class CNCMHDoc : public CDocument
{
protected: // create from serialization only
	CNCMHDoc() noexcept;
	DECLARE_DYNCREATE(CNCMHDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	CMapStringToNStock* NGetExObjList();
	bool IsJustLoaded() const;
	void ResetJustLoaded();
	CStocksCollection& GetStocksList(void) { return NStocksList; }
	auto GetpElems() const { return pElems; }
	int GetElemsSize() const { return ElemsSize; }


// Implementation
public:
	virtual ~CNCMHDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BImageRGB ImageRGB;
	class NCMData* pExternData;
	CMapStringToNStock NExObjList;
	CStocksCollection NStocksList;
	RLines::LineVertex* pElems;
	int ElemsSize;
	bool JustLoaded;
	NCMApplicationType AppType = AT_MILL;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
