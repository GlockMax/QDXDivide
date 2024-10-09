#pragma once
#include "BPoint.h"
class DDistance
{
public:
	DDistance();
	bool IsActive() const;
	void SetActive(bool set);
	void SetSnapPoint(const BPoint& p);
	void SetClickPoint();
	void SetLastPoint(const BPoint& p);
	void Draw();
	void DrawLine(const BPoint& p0, const BPoint& p1) const;
	void DrawRubberLine(const BPoint& p0, const BPoint& p1) const;
	const BPoint& GetBase() const;
	const BPoint& GetSnap() const;
protected:
	bool active_;
	BPoint base_;
	BPoint snap_;
	BPoint last_;
};

