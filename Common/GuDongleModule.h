#pragma once
#include "NCMB_API.h"

class NCMB_API GuDongleModule
{
protected:
    HANDLE  m_hGrd;

public:
	static int DongleVersion;
	GuDongleModule(void);
	~GuDongleModule(void);
	bool Lock(int i);
	bool Unlock(void);
	void Clear(void);
};
