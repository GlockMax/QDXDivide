#include "pch.h"
#include "ResLocal.h"
#include "SMParameter.h"
#include "SMDialog.h"
#include "SMEdit.h"

BEGIN_MESSAGE_MAP(SMEdit, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()

SMEdit::SMEdit(SMParameter& par, class SMDialog* parentDialog) : par_(par), parentDialog_(parentDialog)
{
}

void SMEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (par_.allowAlpha_ || par_.comboBoxEnabled_)
    {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
    else
    {
        // a number
        if (nChar == '.')
        {
            if (par_.decimals_ != 0)
            {
                char lpszBuffer[256];
                auto lineLen = GetLine(0, lpszBuffer, 256);
                for (--lineLen; 0 <= lineLen; --lineLen)
                    if (lpszBuffer[lineLen] == '.')
                        break;
                if (lineLen == -1)
                    CEdit::OnChar(nChar, nRepCnt, nFlags);
                else
                    ShowBalloon(0, IDS_MES_MACRO_BALLOON_POINT);
            }
            else
            {
                ShowBalloon(0, IDS_MES_MACRO_BALLOON_INT);
            }
        }
        else if (nChar == '-' || nChar == '+')
        {
            if (par_.allowNeg_)
            {
                if (CharFromPos(GetCaretPos()) == 0)
                {
                    char lpszBuffer[256];
                    auto lineLen = GetLine(0, lpszBuffer, 256);
                    if (lineLen > 0)
                    {
                        if (lpszBuffer[0] != '-' && lpszBuffer[0] != '+')
                            CEdit::OnChar(nChar, nRepCnt, nFlags);
                        else
                            ShowBalloon(0, IDS_MES_MACRO_BALLOON_POINT);
                    }
                    else
                        CEdit::OnChar(nChar, nRepCnt, nFlags);
                }
            }
            else
            {
                ShowBalloon(0, IDS_MES_MACRO_BALLOON_UNSIGNED);
            }
        }
        else if (isdigit(nChar) || iscntrl(nChar))
        {
            CEdit::OnChar(nChar, nRepCnt, nFlags);
        }
        else
        {
            ShowBalloon(0, IDS_MES_MACRO_BALLOON_POINT);
        }
    }
    GetWindowText(par_.value_);
    // recalc formulas
    if (parentDialog_ != nullptr)
        parentDialog_->RecalcFunctions();
}

void SMEdit::ShowBalloon(UINT id_title, UINT id_text)
{
    EDITBALLOONTIP tip;
    tip.cbStruct = sizeof(EDITBALLOONTIP);
    if (id_title == 0)
    {
        tip.pszTitle = nullptr;
    }
    else
    {
        CString ti;
        ti.LoadStringA(id_title);
        ti += '\0';
        int size = MultiByteToWideChar(CP_THREAD_ACP, 0, ti, ti.GetLength(), NULL, 0);
        wchar_t* wti = new wchar_t[size];
        MultiByteToWideChar(CP_THREAD_ACP, 0, ti, ti.GetLength(), wti, size);
        tip.pszTitle = wti;
    }
    if (id_text == 0)
    {
        tip.pszText = nullptr;
    }
    else
    {
        CString ti;
        ti.LoadStringA(id_text);
        ti += '\0';
        int size = MultiByteToWideChar(CP_THREAD_ACP, 0, ti, ti.GetLength(), NULL, 0);
        wchar_t* wti = new wchar_t[size];
        MultiByteToWideChar(CP_THREAD_ACP, 0, ti, ti.GetLength(), wti, size);
        tip.pszText = wti;
    }
    SendMessage(EM_SHOWBALLOONTIP, NULL, (LPARAM)&tip);
    delete[] tip.pszTitle;
    delete[] tip.pszText;
}
