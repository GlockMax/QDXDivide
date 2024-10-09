#include "stdafx.h"
#include "NColor.h"
#include "NCMProject.h"
#include "ntiparams.h"
#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace std;

extern "C" NCMB_API NTiParams* __stdcall GenNTiParams()
{
	return new NTiParamsInt;
}

NTiParamsInt::NTiParamsInt(void)
{
}

NTiParamsInt::~NTiParamsInt(void)
{
}
double NTiParamsInt::GetDouble( const char * Key, double Default)
{
	string SKey(Key);
	size_t LastPos = SKey.rfind('@');
	if(LastPos == string::npos)
		return Default;
	TiXmlElement * pElement = GetElement(Key, LastPos);
	if(pElement == NULL)
		return Default;
	string SubKey = SKey.substr(LastPos + 1);
	double Res;
	if(pElement->QueryDoubleAttribute(SubKey.data(), &Res) == TIXML_SUCCESS)
		return Res;
	return Default;
}
const char * NTiParamsInt::GetString( const char * Key, const char * Default)
{
	string SKey(Key);
	size_t LastPos = SKey.rfind('@');
	if(LastPos == string::npos)
		return Default;
	TiXmlElement * pElement = GetElement(Key, LastPos);
	if(pElement == NULL)
		return Default;
	string SubKey = SKey.substr(LastPos + 1);
	const char * Res = pElement->Attribute(SubKey.data());
	if(Res)
		return Res;
	return Default;
}

NColor NTiParamsInt::GetColor(const char* Key, const NColor& Default)
{
	string SKey(Key);
	const TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return Default;
	double R = 0., G = 0., B = 0., A = 0.;
	if (pElement->QueryDoubleAttribute(_T("R"), &R) != TIXML_SUCCESS)
		return Default;
	if (pElement->QueryDoubleAttribute(_T("G"), &G) != TIXML_SUCCESS)
		return Default;
	if (pElement->QueryDoubleAttribute(_T("B"), &B) != TIXML_SUCCESS)
		return Default;
	if (pElement->QueryDoubleAttribute(_T("A"), &A) != TIXML_SUCCESS)
		return Default;
	return NColor((float)R, (float)G, (float)B, (float)A);
}

LOGFONT NTiParamsInt::GetFont(const char* Key, const LOGFONT& Default)
{
	//<Font Size = "12" FaceName = "" Height = "" Width = "" Escapement = "" Orientation = "" Weight = "" Italic = "" Underline = "" StrikeOut = "" CharSet = "" OutPrecision = "" Quality = "" PitchAndFamily = "" / >
	string SKey(Key);
	const TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return Default;
	LOGFONT Font;
	int FontSize;
	if (pElement->QueryIntAttribute(_T("Size"), &FontSize) != TIXML_SUCCESS)
		return Default;
	const char* Str = pElement->Attribute(_T("FaceName"));
	if(Str == nullptr)
		return Default;
	strcpy(Font.lfFaceName, Str);
	int Buf = 0;
	Font.lfHeight = -MulDiv(FontSize, GetDeviceCaps(GetDC(AfxGetMainWnd()->GetSafeHwnd()), LOGPIXELSY), 72);

	if (pElement->QueryIntAttribute(_T("Width"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfWidth = Buf;
	if (pElement->QueryIntAttribute(_T("Escapement"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfEscapement = Buf;
	if (pElement->QueryIntAttribute(_T("Orientation"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfOrientation = Buf;
	if (pElement->QueryIntAttribute(_T("Weight"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfWeight = Buf;
	if (pElement->QueryIntAttribute(_T("Italic"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfItalic = Buf;
	if (pElement->QueryIntAttribute(_T("Underline"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfUnderline = Buf;
	if (pElement->QueryIntAttribute(_T("StrikeOut"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfStrikeOut = Buf;
	if (pElement->QueryIntAttribute(_T("CharSet"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfCharSet = Buf;
	if (pElement->QueryIntAttribute(_T("OutPrecision"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfOutPrecision = Buf;
	if (pElement->QueryIntAttribute(_T("Quality"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfQuality = Buf;
	if (pElement->QueryIntAttribute(_T("PitchAndFamily"), &Buf) != TIXML_SUCCESS)
		return Default;
	Font.lfPitchAndFamily = Buf;
	return Font;
}

BPoint NTiParamsInt::GetVector(const char* Key, const BPoint& Default)
{
	string SKey(Key);
	const TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return Default;
	double X = 0., Y = 0., Z = 0., H = 0.;
	if (pElement->QueryDoubleAttribute(_T("X"), &X) != TIXML_SUCCESS)
		return Default;
	if (pElement->QueryDoubleAttribute(_T("Y"), &Y) != TIXML_SUCCESS)
		return Default;
	if (pElement->QueryDoubleAttribute(_T("Z"), &Z) != TIXML_SUCCESS)
		return Default;
	return BPoint(X, Y, Z, H);
}

bool NTiParamsInt::GetBool( const char * Key, bool Default)
{
	const char *str = GetString(Key, Default ? "Yes" : "No");
	return (strncmp(str, "Yes", 3) == 0);
}


TiXmlElement * NTiParamsInt::GetElement(const char * Key, size_t Length)
{
	string SKey(Key);

	TiXmlNode * pElem = this;
	for(size_t CurPos = 0; CurPos < Length; )
	{
		size_t CurEnd = SKey.find('/', CurPos);
		if(CurEnd == string::npos)
			CurEnd = Length;
		string SubKey = SKey.substr(CurPos, CurEnd - CurPos);
		CurPos = CurEnd + 1;

		pElem = pElem->FirstChildElement(SubKey.data());
		
		if(!pElem)
			return NULL;
	}
	return (TiXmlElement *)pElem;
}

bool NTiParamsInt::LoadFile(const char *Name)
{
	return TiXmlDocument::LoadFile(Name);
}

bool NTiParamsInt::SaveFile(const char* Name) const
{
	return TiXmlDocument::SaveFile(Name);
}

void NTiParamsInt::SetColor(const char* Name, const NColor& Col)
{
	string SKey(Name);
	TiXmlElement* pElement = GetElement(Name, SKey.length());
	if (pElement == nullptr)
		return;
	pElement->SetDoubleAttribute("R", (double)Col.R());
	pElement->SetDoubleAttribute("G", (double)Col.G());
	pElement->SetDoubleAttribute("B", (double)Col.B());
	pElement->SetDoubleAttribute("A", (double)Col.A());
}

void NTiParamsInt::SetFont(const char* Name, const LOGFONT& Font)
{
	string SKey(Name);
	TiXmlElement* pElement = GetElement(Name, SKey.length());
	if (pElement == nullptr)
		return;
	// Fonts
	//<Font Size = "12" FaceName = "" Height = "" Width = "" Escapement = "" Orientation = "" Weight = "" Italic = "" Underline = "" StrikeOut = "" CharSet = "" OutPrecision = "" Quality = "" PitchAndFamily = "" / >
	LONG FontSize = -MulDiv(Font.lfHeight, 72, GetDeviceCaps(GetDC(AfxGetMainWnd()->GetSafeHwnd()), LOGPIXELSY));
	pElement->SetAttribute(_T("Size"), FontSize);
	pElement->SetAttribute(_T("FaceName"), Font.lfFaceName);
	pElement->SetAttribute(_T("Height"), Font.lfHeight);
	pElement->SetAttribute(_T("Width"), Font.lfWidth);
	pElement->SetAttribute(_T("Escapement"), Font.lfEscapement);
	pElement->SetAttribute(_T("Orientation"), Font.lfOrientation);
	pElement->SetAttribute(_T("Weight"), Font.lfWeight);
	pElement->SetAttribute(_T("Italic"), Font.lfItalic);
	pElement->SetAttribute(_T("Underline"), Font.lfUnderline);
	pElement->SetAttribute(_T("StrikeOut"), Font.lfStrikeOut);
	pElement->SetAttribute(_T("CharSet"), Font.lfCharSet);
	pElement->SetAttribute(_T("OutPrecision"), Font.lfOutPrecision);
	pElement->SetAttribute(_T("Quality"), Font.lfQuality);
	pElement->SetAttribute(_T("PitchAndFamily"), Font.lfPitchAndFamily);
}

void NTiParamsInt::SetVector(const char* Name, const BPoint& Vector)
{
	string SKey(Name);
	TiXmlElement* pElement = GetElement(Name, SKey.length());
	if (pElement == nullptr)
		return;
	pElement->SetDoubleAttribute("X", Vector.GetX());
	pElement->SetDoubleAttribute("Y", Vector.GetY());
	pElement->SetDoubleAttribute("Z", Vector.GetZ());
}

void NTiParamsInt::SetDouble(const char* Key, double Val)
{
	string SKey(Key);
	size_t LastPos = SKey.rfind('@');
	if (LastPos == string::npos)
		return;
	TiXmlElement* pElement = GetElement(Key, LastPos);
	if (pElement == nullptr)
		return;
	string SubKey = SKey.substr(LastPos + 1);
	pElement->SetDoubleAttribute(SubKey.data(), Val);
}

void NTiParamsInt::SetInt(const char* Key, int Val)
{
	string SKey(Key);
	size_t LastPos = SKey.rfind('@');
	if (LastPos == string::npos)
		return;
	TiXmlElement* pElement = GetElement(Key, LastPos);
	if (pElement == nullptr)
		return;
	string SubKey = SKey.substr(LastPos + 1);
	pElement->SetAttribute(SubKey.data(), Val);
}

void NTiParamsInt::SetString(const char* Key, const char* Val)
{
	string SKey(Key);
	size_t LastPos = SKey.rfind('@');
	if (LastPos == string::npos)
		return;
	TiXmlElement* pElement = GetElement(Key, LastPos);
	if (pElement == nullptr)
		return;
	string SubKey = SKey.substr(LastPos + 1);
	pElement->SetAttribute(SubKey.data(), Val);
}

void NTiParamsInt::GetMiskDouble(const char* Key, CMapStringToDoublePtr& Doubles)
{
	string SKey(Key);
	TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return;
	for (auto Att = pElement->FirstAttribute(); Att; Att = Att->Next())
	{
		double* pDouble = nullptr;
		if (Doubles.Lookup(Att->Name(), pDouble))
			*pDouble = Att->DoubleValue();
		else
			Doubles[Att->Name()] = new double(Att->DoubleValue());
	}
}

void NTiParamsInt::GetMiskInt(const char* Key, CMapStringToIntPtr& Ints)
{
	string SKey(Key);
	TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return;
	for (auto Att = pElement->FirstAttribute(); Att; Att = Att->Next())
	{
		int* pInt = nullptr;
		if (Ints.Lookup(Att->Name(), pInt))
			*pInt = Att->IntValue();
		else
			Ints[Att->Name()] = new int(Att->IntValue());
	}
}

void NTiParamsInt::GetMiskColors(const char* Key, CMapStringToNColor& Colors)
{
	string SKey(Key);
	TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return;
	for(auto pChild = pElement->FirstChildElement(); pChild != nullptr; pChild = pChild->NextSiblingElement())
	{
		CString ID(pChild->Value());
		ID.Delete(0, 5); // 5 is length of Color word
		double R = 0., G = 0., B = 0., A = 0.;
		if (pChild->QueryDoubleAttribute(_T("R"), &R) != TIXML_SUCCESS)
			continue;
		if (pChild->QueryDoubleAttribute(_T("G"), &G) != TIXML_SUCCESS)
			continue;
		if (pChild->QueryDoubleAttribute(_T("B"), &B) != TIXML_SUCCESS)
			continue;
		if (pChild->QueryDoubleAttribute(_T("A"), &A) != TIXML_SUCCESS)
			continue;
		CString Name;
		Name.LoadString(atoi(ID));
		if (NCM_PROJECT.TrCurBase.Lookup(Name, Name))
		{
			NColor* pCol = nullptr;
			if (Colors.Lookup(Name, pCol))
				*pCol = NColor((float)R, (float)G, (float)B, (float)A);
			else
				Colors[Name] = new NColor((float)R, (float)G, (float)B, (float)A);
		}
	}
}

void NTiParamsInt::AddElement(const char* Key, const char* Name)
{
	string SKey(Key);
	TiXmlElement* pElement = GetElement(Key, SKey.length());
	if (pElement == nullptr)
		return;
	TiXmlElement* pNewElem = new TiXmlElement(Name);
	pElement->LinkEndChild(pNewElem);
}
