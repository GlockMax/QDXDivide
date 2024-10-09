#include "stdafx.h"
#include "sent.h"
#include ".\sent.h"


//----------------------------------------------------
BOOL HK_MNG::WriteLicFile(CString lpFileName)
{	
	CStdioFile file;
	if(!file.Open(lpFileName, CFile::modeCreate|CFile::modeWrite))
		return FALSE;  

	file.WriteString(lic_input->prefix + "\n");
	file.WriteString(lic_input->codestr + "\n");
	file.WriteString(lic_input->config + "\n");

	return TRUE;
}
//----------------------------------------------------
BOOL HK_MNG::ReadLicFile(CString lpFileName)
{
	lic_file = new LicInfo();	
	CStdioFile file;
	if(!file.Open(lpFileName, CFile::modeRead))
		return FALSE; 

	if(!file.ReadString(lic_file->prefix))
		return FALSE;
	if(lic_file->prefix.GetLength() != 3)
		return FALSE;

	if(!file.ReadString(lic_file->codestr))
		return FALSE;

	if(!file.ReadString(lic_file->config))
		return FALSE;
	if(lic_file->config.GetLength() != 16)
		return FALSE;

	return TRUE;
}
//----------------------------------------------------
HK_MNG::HK_MNG() 
{
	lic_input = new LicInfo;
	lic_file = new LicInfo;
	key_input = new LicInfo;
	ZeroMemory(ApiPacket, sizeof(ApiPacket));
	index = 0;
}
//----------------------------------------------------
HK_MNG::~HK_MNG()
{
	RNBOsproCleanup();
	ZeroMemory(lic_input, sizeof(lic_input));
	ZeroMemory(lic_file,  sizeof(lic_input));
	ZeroMemory(key_input, sizeof(lic_input));
	ZeroMemory(ApiPacket, sizeof(ApiPacket));
}
//----------------------------------------------------
void HK_MNG::SaveLicFile()
{
	static char BASED_CODE szFilter[] = "Lic files (*.lic)|*.lic|All Files (*.*)|*.*||";
	CFileDialog *dlg = new CFileDialog(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT,szFilter, AfxGetMainWnd());
	if(dlg->DoModal() == IDOK)
	{
		if(!WriteLicFile(dlg->GetFileName()))
		return;	
	}
}
//----------------------------------------------------
void HK_MNG::LoadLicFile()
{
	static char BASED_CODE szFilter[] = "Lic files (*.lic)|*.lic|All Files (*.*)|*.*||";
	CFileDialog *dlg = new CFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY,szFilter, AfxGetMainWnd());
	if(dlg->DoModal() == IDOK)
	{
		if(!ReadLicFile(dlg->GetFileName()))
		return;
	}
}
//----------------------------------------------------
int HK_MNG::ReadSN()
{
//	KEY_SN = 2222;
//	return SP_SUCCESS;// :TST:
    return RNBOsproRead(&ApiPacket, 0x00, &KEY_SN);
}
//----------------------------------------------------
BOOL CHK_STR(char *s1, char *s2, DWORD size_chk)
{
	for(DWORD i=0; i < size_chk; i++)
	{
		if(s1[i] != s2[i]) 
			return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------
int HK_MNG::InitKey()
{ 
	CONF = lic_file->config;
	memset(&ApiPacket, 0, sizeof(ApiPacket));
	if((status = RNBOsproFormatPacket(&ApiPacket, sizeof(ApiPacket)))!=SP_SUCCESS)
		return status;
	if((status = RNBOsproInitialize(&ApiPacket))!=SP_SUCCESS)
		return status;
	if((status = RNBOsproFindFirstUnit(&ApiPacket, DEV_ID))/*:TST:*/==SP_SUCCESS)
	{
	    if(ReadSN() == SP_SUCCESS)
		{
			char buf[10]; memset(buf, 0, /*strlen(buf)//EK*/10);
			sprintf(buf, "%d", KEY_SN);
			key_input->SetStruct(DEV_ID,(CString)buf, CONF);
			if (key_input->codestr == lic_file->codestr && key_input->config == lic_file->config)
			{
				Found = TRUE;
				return SP_SUCCESS;
			}
			else
			{
				while(status != SP_SUCCESS && index < MAX_NUM_DEV)
				{
					status = RNBOsproFindNextUnit(&ApiPacket);
					index++;
					if(status == SP_SUCCESS && ReadSN() == SP_SUCCESS)
					{
						char buf[10];
						sprintf(buf, "%d", KEY_SN); memset(buf, 0, strlen(buf));
						key_input->SetStruct(DEV_ID,(CString)buf, CONF);
						if (key_input->codestr == lic_file->codestr && key_input->config == lic_file->config)
						{
							Found = TRUE;
							return SP_SUCCESS;
						}
					}	
				}		  
			}
		}
	}// end: FindFirst
	Found = FALSE;
	return SP_UNIT_NOT_FOUND;
}
//----------------------------------------------------
LicInfo *HK_MNG::GetLicFromData(int KeySN, CString CFG)
{
    LicInfo *lic = new LicInfo;
	if (KeySN) 
	{
		char buf[10];
		sprintf(buf, "%d", KeySN);
		lic->SetStruct(DEV_ID, buf, CFG);		  
		return lic;
	}
	return NULL;
}
//----------------------------------------------------
RB_WORD HK_MNG::GetKEY_SN(void)
{
	return KEY_SN;
}
