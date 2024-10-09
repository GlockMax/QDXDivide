
// NCMDoc.cpp : implementation of the CNCMDoc class
//

#include "stdafx.h"
#include "framework.h"
#include "NCMProject.h"
#include "NCMVersion.h"
#include "NCMData.h"
#include "NCMComData.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.

#include "NCMHDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNCMDoc

IMPLEMENT_DYNCREATE(CNCMHDoc, CDocument)

BEGIN_MESSAGE_MAP(CNCMHDoc, CDocument)
END_MESSAGE_MAP()


// CNCMDoc construction/destruction

CNCMHDoc::CNCMHDoc() noexcept
{
	pExternData = nullptr;
	JustLoaded = true;
	pElems = nullptr;
	ElemsSize = 0;
	AppType = AT_MILL;
}

CNCMHDoc::~CNCMHDoc()
{
	delete pExternData;
	delete[] pElems;
}

BOOL CNCMHDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CNCMDoc serialization

void CNCMHDoc::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		JustLoaded = true;
		CStringA Title("NCM File.");
		CStringA v(" Ver");
		CStringA str = Title;
		SerializeElements(ar, &str, 1);
		if (str != Title)
			return;
		SerializeElements(ar, &str, 1);
		if (str.Left(4) != v) 
			return;
		SerializeElements(ar, &str, 1);
		NCM_PROJECT.SetInFileVersion(atof(str));
		if (NCM_PROJECT.GetInFileVersion() > atof(NCMVersion))
			return;
		// for icon handler only
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.3")))
		{
			AppType = AT_MILL;
			SerializeElements(ar, &AppType, 1);
		}
		// END: for icon handler only
		//Read and forget ExternData
		if (pExternData)
			delete pExternData;
		pExternData = new NCMData;
		pExternData->Serialize(ar);
		if (NCM_PROJECT.GetInFileVersion() < atof("5.3"))
			return;
		CDocument::Serialize(ar);
		ImageRGB.Serialize(ar);
		NCMProject::GetProject().Serialize(ar);

		NCMApplicationType buf;
		SerializeElements(ar, &buf, 1);

		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("4.0")))
		{
			bool FastCut5xEnabled;
			SerializeElements(ar, &FastCut5xEnabled, 1);
			if (ar.IsLoading() && NCM_PROJECT.GetInFileVersion() >= atof("4.0") && NCM_PROJECT.GetInFileVersion() < atof("4.5"))
			{
				bool b;
				SerializeElements(ar, &b, 1);
			}
			bool CutEnabled;
			SerializeElements(ar, &CutEnabled, 1);
		}
		if (!(ar.IsLoading() && NCM_PROJECT.GetInFileVersion() < atof("5.2")))
		{
			NExObjList.Serialize(ar);
			NStocksList.Serialize(ar);
			SerializeElements(ar, &ElemsSize, 1);
			delete[] pElems;
			ElemsSize /= sizeof(*pElems);
			if (ElemsSize == 0)
				pElems = nullptr;
			else
			{
				pElems = new RLines::LineVertex[ElemsSize];
				SerializeElements(ar, pElems, ElemsSize);
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CNCMHDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = ImageRGB.xSize;
	bmi.bmiHeader.biHeight = ImageRGB.ySize;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 8 * ImageRGB.PixelSize;

//	SetDIBitsToDevice(dc, 0, 0, 100, 100, 0, 0, 0, 100, data, &bmi, DIB_RGB_COLORS); 
	if (ImageRGB.xSize > 0 && ImageRGB.ySize > 0 && ImageRGB.pImage != nullptr)
	{
		// stride = ((((biWidth * biBitCount) + 31) & ~31) >> 3)
		CRect Dest(lprcBounds);
		double xRatio = double(Dest.Width()) / ImageRGB.xSize;
		double yRatio = double(Dest.Height()) / ImageRGB.ySize;
		double minRatio = fmin(xRatio, yRatio);
		int Width = int(minRatio * ImageRGB.xSize);
		int Height = int(minRatio * ImageRGB.ySize);
		SetStretchBltMode(dc, STRETCH_HALFTONE);
		StretchDIBits(dc, Dest.left + (Dest.Width() - Width) / 2, Dest.top + (Dest.Height() - Height) / 2, Width, Height
			, 0, 0, ImageRGB.xSize, ImageRGB.ySize, ImageRGB.pImage, &bmi, DIB_RGB_COLORS, SRCCOPY);
	}

	CString strType = _T("Mill");
	switch (AppType)
	{
	case AT_TURN:
		strType = _T("Turn");
		break;
	case AT_MILL_TURN:
		strType = _T("MillTurn");
		break;
	case AT_MILL:
		strType = _T("Mill");
		break;
	case AT_WELD:
		strType = _T("Weld");
		break;
	case AT_PUNCH:
		strType = _T("Punch");
		break;
	}
	CString strText;
	strText.Format(_T("V%3.1f %s"), NCM_PROJECT.GetInFileVersion(), strType);

	LOGFONT lf;
	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_LEFT | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CNCMHDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CNCMHDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CNCMDoc diagnostics

#ifdef _DEBUG
void CNCMHDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNCMHDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



CMapStringToNStock* CNCMHDoc::NGetExObjList()
{
	return &NExObjList;
}

void CNCMHDoc::ResetJustLoaded()
{
	JustLoaded = false;
}

bool CNCMHDoc::IsJustLoaded() const
{
	return JustLoaded;
}