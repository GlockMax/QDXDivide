// NColor.cpp: implementation of the NColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NColor::NColor()
{

}

NColor::~NColor()
{

}



const NColor & NColor::operator =(const NColor &NC)
{
 	for(int i=0;i<4;i++)
		array[i] = NC.array[i];
	return NC;
}

bool NColor::operator!=(const NColor & Col)
{
	for (int i = 0; i < 4; ++i)
		if (array[i] != Col.array[i])
			return true;
	return false;
}

void NColor::Serialize(CArchive &ar)
{
 	for(int i=0;i<4;i++)
		if(ar.IsStoring())
			ar<<array[i];
		else
			ar>>array[i];
	
}

NColor::NColor(float R, float G, float B, float A)
{
	array[0]=R;
	array[1]=G;
	array[2]=B;
	array[3]=A;
}

float &NColor::R()
{
	return array[0];
}
float &NColor::G()
{
	return array[1];
}
float &NColor::B()
{
	return array[2];
}
float &NColor::A()
{
	return array[3];
}
const float NColor::R() const
{
	return array[0];
}
const float NColor::G() const
{
	return array[1];
}
const float NColor::B() const
{
	return array[2];
}
const float NColor::A() const
{
	return array[3];
}

COLORREF NColor::GetRGB() const
{
	return RGB(r(),g(),b());
}

const NColor NColor::SetRGB(COLORREF rgb)
{
	R()	= GetRValue(rgb)/255.f; 
	G()	= GetGValue(rgb)/255.f; 
	B()	= GetBValue(rgb)/255.f;
	A() = 1.;
	return *this;
}

unsigned char NColor::r() const
{
	return (unsigned char)(array[0]*255.f);
}
unsigned char NColor::g() const
{
	return (unsigned char)(array[1]*255.f);
}
unsigned char NColor::b() const
{
	return (unsigned char)(array[2]*255.f);
}
unsigned char NColor::a() const
{
	return (unsigned char)(array[3]*255.f);
}

NColor NColor::Inv() const
{
	NColor b;
	for(int i = 0; i < 3; ++i)
		b.array[i] = 1.f - array[i];
	b.array[3] = array[3];
	return b;
}

NColor::NColor(const NColor &NC)
{
 	for(int i = 0; i < 4; ++i)
		array[i] = NC.array[i];
}
