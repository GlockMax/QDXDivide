#pragma once

#include <vector>
#include "RArrow.h"
#include "glee.h"
#include "GL\GL.h"

class BOpenGLCtrl
{
	friend class BOGLCtrlElem;
	class BOGLCtrlElem
	{
		friend class BOpenGLCtrl;
	protected:
		BOGLCtrlElem(BOpenGLCtrl *pParCtrl, UINT MessageID);
		virtual void DoAction() = 0;
		virtual void Draw(bool Hi) = 0;
		virtual bool DoWheelAction(short zDelta) const { return false; }
		void DrawLetter(char  Letter, const BPoint & P0, const BPoint & Ex, const BPoint & Ey);
	protected:
		const BOpenGLCtrl &Ctrl;
		UINT MessageID;
		static const int NOTCH_ANGLE = 10;
		static const int AxisBaseR = 5;
		static const int AxisCylR = 3;
		static const int AxisConeR = 6;
		static const int AxisCylLen = 34;
		static const int AxisConeLen = 16;
		static const int BaseR = 12;
		static const int ShiftAxis = 10;
		static const int ShiftIso = AxisCylLen + AxisConeLen + AxisBaseR;
		static const int ShiftXY = ShiftIso + AxisBaseR + 10;
		double ColorHoverR = 0.0, ColorHoverG = 1.0, ColorHoverB = 1.0;
		double ColorBaseR = 1., ColorBaseG = 1., ColorBaseB = 0.;
		double ColorAxisXR = 1.0, ColorAxisXG = 0.5, ColorAxisXB = 0.5;
		double ColorAxis_XR = 1.0, ColorAxis_XG = 0.8, ColorAxis_XB = 0.8;
		double ColorAxisYR = 0.5, ColorAxisYG = 1.0, ColorAxisYB = 0.5;
		double ColorAxis_YR = 0.8, ColorAxis_YG = 1.0, ColorAxis_YB = 0.8;
		double ColorAxisZR = 0.5, ColorAxisZG = 0.5, ColorAxisZB = 1.0;
		double ColorAxis_ZR = 0.8, ColorAxis_ZG = 0.8, ColorAxis_ZB = 1.0;
	public:
		UINT GetMessageID() const { return MessageID; }
	};
public:
	BOpenGLCtrl(class BOpenGLWnd *pP);
	virtual ~BOpenGLCtrl();
	bool OnLButtonDown(UINT nFlags, const CPoint & point);
	bool OnLButtonUp(UINT nFlags, const CPoint & point);
	bool OnMouseMove(UINT nFlags, const CPoint & point);
	bool OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void Draw();
	static double GetZoneXSize() {	return ZoneXSize;}
	static double GetZoneYSize() {	return ZoneYSize;}
protected:
	bool HitTest(const CPoint &point);
	void CalcScale();
protected:
	class BOpenGLWnd *pParent;
	bool Active;
	int SelectedInd;
	CRect GabarRect;
	double Scale;
	GLdouble X0, Y0, Z0;
	static const int ZoneXSize = 200;// Active zone sizes in pixels
	static const int ZoneYSize = 200;
	static const int ShiftX = 80;// Begin of coord
	static const int ShiftY = 80;
	std::vector<BOGLCtrlElem *> Elems;
protected:
	class ElemMain : public BOGLCtrlElem
	{
	public:
		ElemMain(BOpenGLCtrl *pParCtrl, UINT MessageID) : BOGLCtrlElem(pParCtrl, MessageID) {};
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowZ : public BOGLCtrlElem
	{
	public:
		ElemArrowZ(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowX : public BOGLCtrlElem
	{
	public:
		ElemArrowX(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowY : public BOGLCtrlElem
	{
	public:
		ElemArrowY(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrow_Z : public BOGLCtrlElem
	{
	public:
		ElemArrow_Z(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};


	class ElemArrow_X : public BOGLCtrlElem
	{
	public:
		ElemArrow_X(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrow_Y : public BOGLCtrlElem
	{
	public:
		ElemArrow_Y(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowRotZ : public BOGLCtrlElem
	{
	public:
		ElemArrowRotZ(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowRot_Z : public BOGLCtrlElem
	{
	public:
		ElemArrowRot_Z(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowRotX : public BOGLCtrlElem
	{
	public:
		ElemArrowRotX(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowRot_X : public BOGLCtrlElem
	{
	public:
		ElemArrowRot_X(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};


	class ElemArrowRotY : public BOGLCtrlElem
	{
	public:
		ElemArrowRotY(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowRot_Y : public BOGLCtrlElem
	{
	public:
		ElemArrowRot_Y(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowIsoX : public BOGLCtrlElem
	{
	public:
		ElemArrowIsoX(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowIsoY : public BOGLCtrlElem
	{
	public:
		ElemArrowIsoY(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowIso_X : public BOGLCtrlElem
	{
	public:
		ElemArrowIso_X(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowIso_Y : public BOGLCtrlElem
	{
	public:
		ElemArrowIso_Y(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowIsoZ : public BOGLCtrlElem
	{
	public:
		ElemArrowIsoZ(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowIso_Z : public BOGLCtrlElem
	{
	public:
		ElemArrowIso_Z(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
		virtual bool DoWheelAction(short zDelta) const override;
	protected:
		RArrow Arrow;
	};

	class ElemArrowLetXYZ : public BOGLCtrlElem
	{
	public:
		ElemArrowLetXYZ(BOpenGLCtrl *pParCtrl, UINT MessageID);
	protected:
		virtual void DoAction() override;
		virtual void Draw(bool Hi) override;
	};


};

