// MTPart.h: interface for the MTPart class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "PAbstractObject.h"
#include "NPassiveObject.h"
#include "NColor.h"
#include "BStl.h"

class MTPartAxis;
/// Класс детали.
/**
Хранит в себе stl, ссылку на MTPartAxis, ассоциированный с ним.
\sa MTPartAxis, BStl
*/
class AFX_EXT_CLASS MTPart : public NPassiveObject, public PAbstractObject  
{
public:
	/// Получение габаритного параллелепипеда
	BBox GetGabar(const class MTConfig& Conf) const;
	const CString& GetFile() const;

	const CString &GetAxisName() const;
	void SetAxisName(const CString &NewName);
	MTPart();
	void LoadFromStl(const CString& n);
	void Draw(const class MTConfig& Conf, enum RenderMode GlobRMode, enum RenderMode RMode, bool DrawSharpEdges, bool DrawSurf,  bool DrawSmoothSurf);
	const class MTSpinAxis* GetSpinAncestor(const MTConfig& Conf) const;
	/// Нарисовать габариты в виде ребер прямоугольного параллелепипеда
	/** Используется для выделения выбранной детали в дереве в режиме редактирования станка \sa MTEdit */
	void DrawGabar(const class MTConfig& Conf);
	MTPart(const MTPartAxis *NAx);
	virtual ~MTPart();
	double GetMargin(void) const { return CollisionMargin;}
	void SetMargin(double Margin) { CollisionMargin = Margin;}
	DECLARE_SERIAL(MTPart)

protected:
	/// Имя оси в дереве станка, с которой ассоциируется деталь
	CString AxisName;
	/// Указатель на ось, с которой ассоциируется деталь.
	/* При изменениях в станке необходимо обновлять этот указатель. 
	Такое обновление выполянется в методе MTMachineTool::Reset() */
	NColor Color;
	double CollisionMargin;
	/// Геометрическая модель
	BStl Stl;
	bool Visible;
	bool WFrame;
	class RSpinning* pSpinRend;
	size_t SpinRendVersion;// for spinnable parts only
public:
	bool GetWFrame(void) const { return WFrame;}
	class MTPartVisProp *GetPartProp();
	void SetPartProp(class MTPartVisProp *pVisProp);
	const BMatr GetMatr(const class MTConfig& Conf) const;
	BMatr GetCollMatr(const class MTConfig& Conf, const float*& Triangles, size_t& SRVersion);
	BMatr GetCollMatr(const class MTConfig& Conf) const;
	void SetColor(const NColor & Col);
	NColor &GetColor();
	const GLfloat * GetTriangles(const class MTConfig& Conf, size_t& SRVersion);
	const BPoint RayCasting(const class MTConfig& Conf, const BPoint & P, const BPoint & V, double MinW, BPoint &N) const;
    bool FindSegmentsWithStl(const class MTConfig& Conf, SnapRopes& oSegments, std::vector<BMatr>& oMatrsInvr);
	void SetEdgesAttrs(const class BEdgesAttrs &Attr) override { NPassiveObject::SetEdgesAttrs(Attr);  Stl.SetEdgesAttrs(Attr); }
	void SetVisible(bool Vis = true) { Visible = Vis;}
	bool IsVisible(void) const { return Visible;}
	void SetWFrame(bool WFr = true) { WFrame = WFr;}
	bool IsWFrame(void) const { return WFrame;}
	bool IsActualSpinRend(bool CanMoveWhileSpin, const BMatr& RelMatr) const;
	bool ReCreateSpinRand(const MTPartAxis* pAxis, const MTSpinAxis* pSpinAxis);

	virtual class PropList * Create(class PropList *) override;
	virtual class PropList * Change(class PropList *, class PropSingle *, int, enum UpdateModes &AdditionalModes, int &Group) override;
};
