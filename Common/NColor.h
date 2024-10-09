// NColor.h: interface for the NColor class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "NCMB_API.h"
#include "MElemIDProc.h"

class NCMB_API NColor
{
public:
	NColor(const NColor &NC);
	NColor Inv() const;
	unsigned char r() const;
	unsigned char g() const;
	unsigned char b() const;
	unsigned char a() const;
	const NColor SetRGB(COLORREF rgb);
	COLORREF GetRGB() const;
	float &R();
	float &G();
	float &B();
	float &A();
	const float R() const;
	const float G() const;
	const float B() const;
	const float A() const;
	NColor(float R,float G,float B,float A);
	void Serialize(class CArchive &ar);
	float array[4];
	NColor();
	virtual ~NColor();
	const NColor & operator = (const NColor &NC);
	float GetA() const { return array[3];}
	bool EqualTo(PackedColor PC) const { return (PC.fr == r()) && (PC.fg == g()) && (PC.fb == b());}
	bool operator != (const NColor & Col);
};
