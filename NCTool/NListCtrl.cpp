// NListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_EDITLABEL 1

/////////////////////////////////////////////////////////////////////////////
// NListCtrl

NListCtrl::NListCtrl()
{
	m_bColumnLines = FALSE;
	m_newStyle = -1;
}

NListCtrl::~NListCtrl()
{
	for (int i=0; i<m_arLVEdit.GetSize(); i++)
		delete m_arLVEdit.GetAt(i);
	m_arLVEdit.RemoveAll();

}


BEGIN_MESSAGE_MAP(NListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(NListCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NListCtrl message handlers

// установить/снять стиль отображения редактируемой колонки
// в виде прямоугольника
void NListCtrl::SetRectStyle(BOOL bRect)
{
	m_bColumnLines = bRect;
}

void NListCtrl::AddNewStyle(DWORD dwNewStyle)
{
	m_newStyle = dwNewStyle;
}

// добавление колонки доступноя для изменения
void NListCtrl::AddChangingColumn(int col)
{
	bool f = false;

	if (m_arChangeCol.GetSize() == 0)
		f = true;
	else
		for (int i=0; i<m_arChangeCol.GetSize(); i++)
			if (m_arChangeCol.GetAt(i) == col)
			{
				f = true;
				i = int(m_arChangeCol.GetSize());
			}
	if (f)
		m_arChangeCol.Add(col);
}

// отображение редактируемой колонки в виде прямоугольника
/*void NListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (!m_bColumnLines)
		return;

	CRect		rect;
	CSize		sz(3,2);
    CPen       *pOldPen;
    CPen        pen;
    CGdiObject *pOldBrush;

    pen.CreatePen( PS_SOLID, 0, RGB(0,0,0) );
    pOldPen   = dc.SelectObject(&pen);
    pOldBrush = dc.SelectStockObject(NULL_BRUSH);

	for (int i=0; i<GetItemCount(); i++)
		for (int j=0; j<m_arChangeCol.GetSize(); j++)
		{
			GetSubItemRect(i, m_arChangeCol.GetAt(j), LVIR_LABEL, rect);
			rect -= sz;
			dc.Rectangle(rect);
		}	
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}*/


void NListCtrl::InsertEditLAbels()
{
	for (int i=0; i<m_arLVEdit.GetSize(); i++)
		delete m_arLVEdit.GetAt(i);
	m_arLVEdit.RemoveAll();

	if (!m_bColumnLines)
		return;

	CRect rcItem;
	CEdit *pEdit;
	CFont *pFont = GetFont();

	for (int i=0; i<GetItemCount(); i++)
	{
		for (int j=0; j<m_arChangeCol.GetSize(); j++)
		{
			GetSubItemRect(i,m_arChangeCol.GetAt(j),LVIR_LABEL,rcItem);
			
			pEdit = new CEdit;
			rcItem.bottom = rcItem.bottom - 1;

			pEdit->Create(WS_CHILD|WS_BORDER|WS_VISIBLE|ES_LEFT, 
				rcItem, this, i+j);
			pEdit->SetWindowText(GetItemText(i,m_arChangeCol.GetAt(j)));
			pEdit->SetFont(pFont);
			
			if (m_newStyle != -1)
				pEdit->ModifyStyle(0, m_newStyle, SWP_NOMOVE|SWP_NOSIZE);
			
			m_arLVEdit.Add(pEdit);
		}
	}
}

int NListCtrl::GetNumEditColumn()
{
	return int(m_arChangeCol.GetSize());
}

CEdit* NListCtrl::GetEditCtrl(int index)
{
	if (m_arLVEdit.GetSize() <= 0)
		return NULL;

	return m_arLVEdit.GetAt(index);
}
