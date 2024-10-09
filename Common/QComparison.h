#pragma once
#include "QComparisonDlg.h"
#include "SimpleCloud.h"
#include "MElemIDProc.h"
#include "SimpleMesh.h"

//определим цвета по умолчанию
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
	void Verification(bool Recalc, bool Recreate);//верификация расстояний
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
	//верификация расстояний
	void VeriCalc(void);//функция потока вычислений
    void VeriFinish(void);//функция по заврешению вычислений
    static UINT VeriThread(LPVOID cmp);//функция для создания потока вычислений
	void DeActivate(void);
	
	bool IsNewColor(void);//true-цвет кнопок изменился; false-цвет кнопок прежний
	bool IsNewLim(void);////true-пределы диапазонов изменились; false-нет
	void SetNewColor(void);//записать новый цвет кнопок во временный массив для дальнейших сравнений на изменение цвета
	void SetNewLim(void);//записать новые границы диапазонов во временный массив для дальнейших сравнений
	void SetNewColorRender(float ***A);//Изменение цветов в самой заготовке
	unsigned int NumColor(COLORREF c);//возвр. ном. диап., с которым совпал цвет до изменения цветов кнопок
	void Clear(float ***&A);//очистка указателей без удаления содержимого
		
	bool IsActive(void) const;
	unsigned int Diapason(double dist);
	double GetMaxDist();
	int GetMaxNumber() const { return Dlg.MaxNumber; }
	//возвращает номер цветового диапазона, в который попала точка заготовки 
    bool Inside(const int i, const int j, const int k);//определяет принадлежность ячейки внутреннему полупространству заготовки
	bool Smooth;//Хранится значение Smooth тестового диалога
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
	COLORREF *def_Color, *Color_tmp;//цвет по умолчанию
	double *Lim_tmp;//массив границ диапазонов до изменения
	class NStock* pStock;
	class GQuadRenderMill *QRender;
	class GQuadRenderDX5X *DRender;
	float Dist2Color(double Dist);

	bool Active;//активирован ли объект
    bool Terminate;//следует ли завершить поток вычисления
    bool ThreadActive;//активен ли поток вычисления
	QComparisonDlg Dlg;//Диалог
	CWnd *pParent;
    CWinThread* VerTh;//указатель на вычислительный поток
	
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
