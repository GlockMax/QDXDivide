#pragma once
#include <afxwin.h>
class SMEdit : public CEdit
{
public:
    SMEdit::SMEdit(class SMParameter& par, class SMDialog* parentDialog);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
    void ShowBalloon(UINT id_title, UINT id_text);
protected:
    DECLARE_MESSAGE_MAP()
protected:
    class SMParameter& par_;
    class SMDialog* parentDialog_;
};

