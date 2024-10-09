#include "pch.h"
#include "license.h"
#include "MainDlg.h"
#include <iostream> 
#using <System.Xml.dll>

using namespace System;
using namespace System::Xml;
using namespace std;
using namespace System::Text;

int  license::createXML(String^ xmlName, CString clientName, CString clientID)
{
	XmlDocument^ doc = gcnew XmlDocument;
	doc->LoadXml("<LicenseFile><ClientsInfo></ClientsInfo></LicenseFile>");

	XmlElement^ cliname = doc->CreateElement("ClientName");
	string str = clientName;
	String^ str2 = gcnew String(str.c_str());
	cliname->InnerText = str2;
	doc->DocumentElement->LastChild->AppendChild(cliname);

	XmlElement^ cliID = doc->CreateElement("ClientID");
	str = clientID;
	str2 = gcnew String(str.c_str());
	cliID->InnerText = str2;
	doc->DocumentElement->LastChild->AppendChild(cliID);

	XmlElement^ file = doc->CreateElement("Files");

	XmlElement^ PPfilename = doc->CreateElement("FileName");
	doc->DocumentElement->LastChild->AppendChild(file);

	XmlTextWriter^ writer = gcnew XmlTextWriter(xmlName, gcnew UTF8Encoding(false));

	writer->Formatting = Formatting::Indented;
	doc->Save(writer);
	writer->Close();

	return 0;

}

bool license::deleteAll(String^ xmlName) 
{
	XmlDocument^ doc = gcnew XmlDocument;
	doc->Load(xmlName);

	XmlNodeList^ ppFileList = doc->GetElementsByTagName("Files");
	XmlElement^ ppFileElement = safe_cast<XmlElement^>(ppFileList->Item(0));
	if (ppFileElement)
	{
		ppFileElement->RemoveAll();
		
	}
	doc->Save(xmlName);
	return false;
}


bool license::addXML(String^ xmlName, CString ppFileName)
{
	XmlDocument^ doc = gcnew XmlDocument;
	doc->Load(xmlName);

	XmlNodeList^ ppFileList = doc->GetElementsByTagName("Files");
	XmlElement^ ppFileElement = safe_cast<XmlElement^>(ppFileList->Item(0));
	if (!ppFileElement)		//there is no tag "Files" in license file
	{
		return false;
	}
	

	for (int i = 0; i < ppFileElement->ChildNodes->Count; i++)
	{
		if ((ppFileElement->ChildNodes[i]->InnerText) == ppFileName)
		{
			return false;
		}
	};

	XmlElement^ PPfilename = doc->CreateElement("FileName");
	string str = ppFileName;
	String^ str2 = gcnew String(str.c_str());
	PPfilename->InnerText = str2;
	doc->DocumentElement->LastChild->LastChild->AppendChild(PPfilename);

	XmlTextWriter^ writer = gcnew XmlTextWriter(xmlName, gcnew UTF8Encoding(false));

	writer->Formatting = Formatting::Indented;
	doc->Save(writer);
	writer->Close();

	return true;

}


bool license::check_license(CListBox* lbox, std::vector<CString> &vector_ppfiles, CString pathName, CString clientName, CString clientID)
{
	string file_name = pathName + "\\" + clientName + ".xml";
	String^ xmlName = gcnew String(file_name.c_str());
	if (System::IO::File::Exists(xmlName))
	{
		XmlDocument^ doc = gcnew XmlDocument;

		CFile file;
		string tagsize = "<?xml version=10 encoding=utf - 8?><LicenseFile><ClientsInfo><ClientName>< / ClientName><ClientID>< / ClientID><Files / >< / ClientsInfo>< / LicenseFile>";
		
		if (!file.Open((LPCTSTR)file_name.data(), CFile::modeRead || CFile::typeBinary, NULL) || (file.GetLength()<tagsize.length()))
		{
			return false;
		}
		file.Close();

		doc->Load(xmlName);
		
		XmlNodeList^ ppFileList = doc->GetElementsByTagName("Files");

		if (!ppFileList)
		{
			return false;
		}

		XmlElement^ ppFileElement = safe_cast<XmlElement^>(ppFileList->Item(0));

		int a = ppFileElement->ChildNodes->Count;

		for (int i = 0; i < ppFileElement->ChildNodes->Count; i++)
		{
			CString file = ppFileElement->ChildNodes[i]->InnerText;
			lbox->AddString(file);
			vector_ppfiles.push_back(file);
		}
		doc->Save(xmlName);

	}
	else
	{
		createXML(xmlName, clientName, clientID);
	}

	return true;
}
