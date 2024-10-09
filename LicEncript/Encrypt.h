#pragma once
//#define _AFXDLL

#include <afx.h>
#include <windows.h>
#include <Wincrypt.h>
class Encrypt
{
private:
	bool Encryption(BYTE* buffer, DWORD& lenData);

public:
	bool CreateNewFile(LPCTSTR inputFileName, LPCTSTR outputFileName);
	void SignXmlFile(System::String^ FileName, System::String^ SignedFileName);

};

