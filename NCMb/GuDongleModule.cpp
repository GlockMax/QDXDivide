#include "stdafx.h"
#include "..\guard\Include\GrdApi.h"
#include "GuDongleModule.h"
#define	CryptPU		0x8566783lu							// Random value for access codes security                              
#define	CryptRD		0x17d49c84u							// Random value for access codes security                              
int GuDongleModule::DongleVersion = 0;
// Data variables 
DWORD	dwPublic1 = 0x717C1CBC-CryptPU;				// Must be encoded             
DWORD	dwPrivRD1 = 0xF9AF78F6-CryptRD;				// Must be encoded             
DWORD	dwPublic2 = 0x70AEA9AA-CryptPU;				// Must be encoded             
DWORD	dwPrivRD2 = 0xB0ABF238-CryptRD;				// Must be encoded             

GuDongleModule::GuDongleModule(void)
{
	m_hGrd = NULL;
}

GuDongleModule::~GuDongleModule(void)
{
	Clear();
}
bool GuDongleModule::Lock(int i)
{
	if(!m_hGrd)
	{
		m_hGrd = GrdCreateHandle( NULL, GrdCHM_SingleThread, NULL );
		// Store dongle codes in Guardant protected container
		int nRet = GrdSetAccessCodes( m_hGrd, (DongleVersion == 0 ? dwPublic1 : dwPublic2) + CryptPU, (DongleVersion == 0 ? dwPrivRD1 : dwPrivRD2) + CryptRD, CryptPU, CryptRD );
		if( nRet != GrdE_OK )
		{		
			return	false;
		}
		// Set System Address Mode (SAM) as default mode
		nRet = GrdSetWorkMode( m_hGrd, GrdWM_UAM, GrdWMFM_DriverAuto );
		if( nRet != GrdE_OK )
			return	false;

		nRet = GrdSetFindMode( m_hGrd, GrdFMR_Remote, GrdFM_SN, 0, 0, 31161, 0, 0, 		GrdDT_Win,
		GrdFMM_ALL,
		GrdFMI_ALL ); 

		if( nRet != GrdE_OK )
			return	false;

		DWORD	dwID;  
		TGrdFindInfo	GrdFindInfo;
		nRet = GrdFind( m_hGrd, GrdF_First, &dwID, &GrdFindInfo );
		if( nRet != GrdE_OK )
			return	false;
	}
	if(!m_hGrd)
		return false;
	int Ret = GrdLogin(m_hGrd, i, GrdLM_PerStation);
	if(Ret != GrdE_OK)
		return false;

	return true;
		
}

bool GuDongleModule::Unlock(void)
{
	if(!m_hGrd)
		return true;
	int Ret = GrdLogout(m_hGrd, 0);
	if(Ret != GrdE_OK)
		return false;
	return true;
}

void GuDongleModule::Clear(void)
{
	if( m_hGrd )
	{
		GrdCloseHandle( m_hGrd );
		m_hGrd = NULL;
	}
}
