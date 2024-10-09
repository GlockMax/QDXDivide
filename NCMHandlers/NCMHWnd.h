#pragma once
#include "BOpenGLWnd.h"
class NCMHWnd :
    public BOpenGLWnd
{
protected:
    void SetZRange();
    BBox GetSceneGabar(bool VisibleOnly) override;
    void DrawScene(RenderMode GlobRMode);
    afx_msg void OnPaint();
public:
    void GetMouseCoord(const CPoint& point, double* wz, double* x, double* y, double* z) override;
    void NUpdate(const CRect* ClipRect = nullptr) override;
    CDocument* GetDocument() override;
    void ClearBackground(void) override;
    DECLARE_MESSAGE_MAP()
};

