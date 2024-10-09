#pragma once

class MVRDS;

class MVRDSmember
{
public:
	MVRDSmember(void){ Undefine();}
	~MVRDSmember(void);
	MVRDSmember(MVRDS *pV){ pVRDS = pV; RemoveActive();}
	bool IsDefined() const { return pVRDS != NULL;}
	void Undefine() { pVRDS = NULL; Active = false;}
	bool IsActive() const { return Active;}
	void SetActive(void) { Active = true;}
	void RemoveActive(void) { Active = false;}

protected:
	MVRDS *pVRDS;
	bool Active;
};
