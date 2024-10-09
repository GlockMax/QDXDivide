#pragma once
#define DEV_ID     24597
#include "spromeps.h"
#include "Security.h"
////////////////////////////////////////////////////////////////////////////////////

class LicInfo
{
public:
	LicInfo ()
	{
	   prefix = "LIC"; 
	};
//-----------------------------------------------------------------------------------------------
	BOOL MDCode(CString buffer)
	{
		if(!buffer)
			return FALSE;
	//EK	  codestr = (char*)MD5((UCHAR*)buffer.GetBuffer(),strlen(buffer.GetBuffer()),NULL);
		unsigned char hash[17];
		hash[16] = '\x0';
		if(!Security::_GetHash((UCHAR*)buffer.GetBuffer(),hash , (int)strlen(buffer.GetBuffer())))
			return FALSE;
		codestr = CString(hash);
		return TRUE;
	}
//-----------------------------------------------------------------------------------------------
	void SetStruct(DWORD devID, CString SN, CString conf)
	{
		char buff[200];	
		config = conf;
		if(!devID || !SN)
			return;
		sprintf(buff,"%d:%s:%s:%s",  devID, prefix.GetBuffer(), SN.GetBuffer(), config.GetBuffer());	 
		MDCode(buff);
	}
public:
	CString prefix;
	CString codestr;
	CString config;
};

////////////////////////////////////////////////////////////////////////////////////////////////
class HK_MNG {
	SP_STATUS status;
	RB_SPRO_APIPACKET ApiPacket;
	int index;
private:		
	RB_WORD KEY_SN;
	CString CONF;
    LicInfo *GetLicFromData(int, CString);
	LicInfo *GetLicFromKey();
public:	
	LicInfo *lic_input;
	LicInfo *key_input;
    LicInfo *lic_file;
    BOOL Found;
/////////////////////////////////////////////////////////////////////	
	HK_MNG(); ~HK_MNG();    	
/////////////////////////////////////////////////////////////////////	
	void SaveLicFile();
	void LoadLicFile();
/////////////////////////////////////////////////////////////////////
 	BOOL ReadLicFile(CString);
	BOOL WriteLicFile(CString); 
    int ReadSN();	
    int InitKey();
	RB_WORD GetKEY_SN(void);
};