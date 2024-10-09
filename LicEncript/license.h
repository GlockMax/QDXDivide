#pragma once
#include <vector>

class license
{
	public:

		int createXML(System::String^ xmlName, CString clientName, CString clientID);
		bool deleteAll(System::String^ xmlName);
		bool addXML(System::String^ xmlName, CString ppFileName);
		bool check_license(CListBox* lbox, std::vector<CString> &vector_ppfiles, CString pathName, CString clientName, CString clientID);

		
};
