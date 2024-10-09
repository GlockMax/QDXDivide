#pragma once
#include "QComparisonDlg.h"
#include "SimpleCloud.h"
#include "MElemIDProc.h"
#include "SimpleMesh.h"

//��������� ����� �� ���������
#define COL0 RGB(0,0,128);
#define COL1 RGB(0,0,255);
#define COL2 RGB(160,255,160);
#define COL3 RGB(0,255,0);
#define COL4 RGB(0,128,0);
#define COL5 RGB(128,128,0);
#define COL6 RGB(255,255,0);
#define COL7 RGB(255,128,0);
#define COL8 RGB(180,90,0);
#define COL9 RGB(255,0,0);
#define COL10 RGB(128,0,0);

class QComparison
{
	friend class QComparisonDlg;
public:
	QComparison(class CNCMDoc& iDoc);
	virtual ~QComparison(void);
	void Activate(CWnd *pWnd, const class CMapStringToNStock *pPartList, class NStock* pStock);
	void SetPickedPoint(const BPoint& P) { PickedPoint = P; }
	void Verification(bool Recalc, bool Recreate);//����������� ����������
	void RepaintCC();
	void RepaintBSP();
	void VeriCalcCC();
	void VeriCalcBSP();
	bool PrepareMeshPointCloud();
	int CalcDistances();
	double Calc1Distace(const BPoint& P);
	void Proc1Distace(const BPoint& P, const BPoint& V);
	void ClearDistance() { Dlg.ClearDistance(); }
	void PreparePointCloudCC();
	void PreparePointCloudBSP();
	static int CalcOctreeLevel(CCLib::SimpleCloud & pointCloud, CCLib::SimpleCloud & meshPointCloud);
	//����������� ����������
	void VeriCalc(void);//������� ������ ����������
    void VeriFinish(void);//������� �� ���������� ����������
    static UINT VeriThread(LPVOID cmp);//������� ��� �������� ������ ����������
	void DeActivate(void);
	
	bool IsNewColor(void);//true-���� ������ ���������; false-���� ������ �������
	bool IsNewLim(void);////true-������� ���������� ����������; false-���
	void SetNewColor(void);//�������� ����� ���� ������ �� ��������� ������ ��� ���������� ��������� �� ��������� �����
	void SetNewLim(void);//�������� ����� ������� ���������� �� ��������� ������ ��� ���������� ���������
	void SetNewColorRender(float ***A);//��������� ������ � ����� ���������
	unsigned int NumColor(COLORREF c);//�����. ���. ����., � ������� ������ ���� �� ��������� ������ ������
	void Clear(float ***&A);//������� ���������� ��� �������� �����������
		
	bool IsActive(void) const;
	unsigned int Diapason(double dist);
	double GetMaxDist();
	int GetMaxNumber() const { return Dlg.MaxNumber; }
	//���������� ����� ��������� ���������, � ������� ������ ����� ��������� 
    bool Inside(const int i, const int j, const int k);//���������� �������������� ������ ����������� ���������������� ���������
	bool Smooth;//�������� �������� Smooth ��������� �������
    bool IsActiveThread() const;
    void TerminateThread(){Terminate=true;}
	bool IsTerminationRequested() const { return Terminate; }
	bool GetInfoTipText(const BPoint& P, CString& Str);
	void GotoCurrentCadr(const cadrID& CadrID) const;
	static bool RayShootingStatic(class NStock* pStock, const BPoint& p, const BPoint& v, bool INOUT, BPoint& FoundP, BPoint& FoundN, ElemID& FoundID, cadrID& FoundCadrID);

protected:
	CCLib::SimpleCloud meshPointCloud;
	CCLib::SimpleMesh* pMesh;
	BPoint PickedPoint;
	COLORREF *def_Color, *Color_tmp;//���� �� ���������
	double *Lim_tmp;//������ ������ ���������� �� ���������
	class NStock* pStock;
	class GQuadRenderMill *QRender;
	class GQuadRenderDX5X *DRender;
	float Dist2Color(double Dist);

	bool Active;//����������� �� ������
    bool Terminate;//������� �� ��������� ����� ����������
    bool ThreadActive;//������� �� ����� ����������
	QComparisonDlg Dlg;//������
	CWnd *pParent;
    CWinThread* VerTh;//��������� �� �������������� �����
	
	unsigned int kolc, kolr;

	const CMapStringToNStock * pActPartList;
	void *pPointCloud;
	bool PolychromeBuf;
	class CNCMDoc& Doc;
public:
	void Draw(void) const;
	void Serialize(CArchive& ar);
    int progress;
};
