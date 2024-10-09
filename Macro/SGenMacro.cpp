#include "pch.h"
#include "WM_APP_messages.h"
#include "ResLocal.h"
#include "ResInv.h"
#include "SMDefiniton.h"
#include "SMDialog.h"
#include "SMGroup.h"
#include "NCMProject.h"
#include "NTiParams.h"
#include "SGenMacro.h"

SGenMacro::SGenMacro() :
	currentPath_(_T("current\\"))
	, origPath_(_T("orig\\"))
	, fileExt_(_T(".mcn"))
	, picturesPath_(_T("orig\\pictures\\"))
	, descriptionsPath_(_T("orig\\descriptions\\"))
	, picturesCimPath_(_T("gfx\\"))
	, descriptionsCimPath_(_T("attachments\\"))
{
	fileExt_.LoadString(IDS_MACRO_MCN);
	dlg_ = nullptr;
	cadrNumStep = 1;
}

SGenMacro::~SGenMacro()
{
	delete dlg_;
}

const CString& SGenMacro::Generate()
{
	delete dlg_;
	dlg_ = new SMDialog;
	dlg_->SetGenMacro(this);
	auto res = dlg_->Create(IDD_MACRO, AfxGetMainWnd());
	dlg_->ShowWindow(SW_SHOW);
	return result_;
}

bool SGenMacro::ImportMacroDefFile(bool oldVersion)
{
	CString baseFilePath;

	fileName_.Empty();
	CString filter("TYP Files(*.typ) | *.* || ");
	CFileDialog fileDialog(TRUE, _T(""), _T(""), OFN_READONLY, _T(""));
	if (fileDialog.DoModal() != IDOK)
		return false;
	fileName_ = fileDialog.GetPathName();
	if (fileName_.IsEmpty())
		return false;
	CStdioFile f;
	if (!f.Open(fileName_, CFile::typeText | CFile::modeRead))
	{
		fileName_.Empty();
		return false;
	}
	// read groups
	 ImportGroups(f);
	// read macros
	bool endFile = false;
	allMacros_.clear();
	while (!endFile)
	{
		allMacros_.emplace_back();
		endFile = !allMacros_.back().Read(f, oldVersion);
	}
	allMacros_.pop_back();
	f.Close();

	int ind = fileName_.ReverseFind('\\');
	if (ind > 0)
		baseFilePath = fileName_.Left(ind + 1);
	fileName_.Delete(0, ind + 1);
	int dotInd = fileName_.ReverseFind('.');
	if (dotInd > 0)
		fileName_ = fileName_.Left(dotInd);
	CString macroPath = NCM_PROJECT.GetMacroPath();
	for each (auto& def in allMacros_)
	{
		if (!def.GetPicture().IsEmpty())
		{
			// copy picture file
			CString pictFilePath = baseFilePath + picturesCimPath_ + def.GetPicture();
			CString destFilePath = macroPath + picturesPath_ + def.GetPicture();
			CopyFile(pictFilePath, destFilePath, false);
		}
		if (!def.GetAttachment().IsEmpty())
		{
			// copy description file
			CString pictFilePath = baseFilePath + descriptionsCimPath_ + def.GetAttachment();
			CString destFilePath = macroPath + descriptionsPath_ + def.GetAttachment();
			CopyFile(pictFilePath, destFilePath, false);
		}
	}
	SaveXML(macroPath + origPath_ + fileName_ + fileExt_);
	SaveXML(macroPath + currentPath_ + fileName_ + fileExt_);
	LoadXML();
	return true;
}

void SGenMacro::ImportGroups(CStdioFile& f)
{
	CString str;
	groups_.clear();
	for (bool end = false; !end; end = !f.ReadString(str))
	{
		if (str.Left(10) == _T("[MACROGRP]"))
		{
			if (!f.ReadString(str))
				break;
			if (str.Left(5) != _T("COUNT"))
				break;
			int i = str.Find('=');
			if (i > 0)
			{
				int count = atoi(str.Mid(i + 1));
				if (count <= 0)
					break;
				groups_.resize(count);
				for (int k = 0; k < count; ++k)
					groups_.at(k).SetAuto(k);
				for (int k = 0; k < count; ++k)
				{
					if (!f.ReadString(str))
						break;
					if (str.Left(3) != _T("GRP"))
						break;
					int i = str.Find('=');
					if (i > 0)
					{
						CString name = str.Mid(i + 1);
						name.Remove(' ');
						name.Remove('\t');
						groups_.at(k).Set(k, name);
					}
				}
			}
		}
	}
	f.SeekToBegin();
}

CString SGenMacro::GetGroupName(int ind) const
{
	if (ind < 0 || ind >= groups_.size())
		return (_T("empty"));
	return groups_.at(ind).GetName();
}

void SGenMacro::ApplyMacro() const
{
	AfxGetMainWnd()->SendMessage(WM_APP_MACRO_APPLY);
	// save macro file
	CString filePath = NCM_PROJECT.GetMacroPath() + currentPath_;
	if (!SaveXML(NCM_PROJECT.GetMacroPath() + fileName_ + fileExt_))
		AfxMessageBox(IDS_MES_MACRO_NOTSAVEMCN);
}

void SGenMacro::InserCadrNums(const CString& NumSym, int LastNum)
{
	int CurNum = LastNum;
	int StartInd = 0;
	for (int i = 0; i < result_.GetLength(); ++i)
	{
		const auto Ind = result_.Find('#', StartInd);
		if (Ind < 0)
			break;
		if (result_.GetLength() > Ind + 1)
			if (result_[Ind + 1] == '#')
			{
				result_.Delete(Ind + 1, 1);// Replace ## -> #
				StartInd = Ind + 1;
				continue;
			}
		// replace # -> cadr number
		CurNum += cadrNumStep;
		result_.Delete(Ind, 1);
		CString Nx;
		Nx.Format(_T("%d"), CurNum);
		result_.Insert(Ind, NumSym + Nx);
	}
}

bool SGenMacro::WriteXML(TiXmlDocument* doc) const
{
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "Windows-1251", "");

	doc->LinkEndChild(decl);

	TiXmlElement* e_GM = new TiXmlElement(_T("genMacro"));
	for each (auto macro in allMacros_)
		macro.WriteXML(e_GM);
	TiXmlElement* e_GR = new TiXmlElement(_T("groups"));
	for each (auto gr in groups_)
		gr.WriteXML(e_GR);
	e_GM->LinkEndChild(e_GR);
	doc->LinkEndChild(e_GM);

	return true;
}

bool SGenMacro::ReadXML(TiXmlDocument* doc)
{
	TiXmlElement* e_GM = doc->FirstChildElement(_T("genMacro"));
	if (e_GM == nullptr)
		return false;

	allMacros_.clear();
	for (TiXmlElement* e_macro = e_GM->FirstChildElement(_T("macro")); e_macro != nullptr; e_macro = e_macro->NextSiblingElement(_T("macro")))
		allMacros_.emplace_back().ReadXML(e_macro);

	groups_.clear();
	TiXmlElement* e_GR = e_GM->FirstChildElement(_T("groups"));
	if (e_GR != nullptr)
	{
		for (TiXmlElement* e_gr = e_GR->FirstChildElement(_T("group")); e_gr != nullptr; e_gr = e_gr->NextSiblingElement(_T("group")))
			groups_.emplace_back().ReadXML(e_gr);
	}
	return true;
}

bool SGenMacro::SaveXML(const CString& path) const
{
	TiXmlDocument doc;
	if (!WriteXML(&doc))
		return false;
	return doc.SaveFile(path);
}

bool SGenMacro::SaveXML() const
{
	if (fileName_.IsEmpty())
		return false;
	return SaveXML(NCM_PROJECT.GetMacroPath() + currentPath_ + fileName_ + fileExt_);
}

bool SGenMacro::LoadXML(const CString& path)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(path))
		return false;
	return ReadXML(&doc);
}

bool SGenMacro::LoadXML()
{
	return LoadXML(NCM_PROJECT.GetMacroPath() + currentPath_ + fileName_ + fileExt_);
}
