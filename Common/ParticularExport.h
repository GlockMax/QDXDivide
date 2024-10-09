#pragma once

class AFX_EXT_CLASS ParticularExport
{
public:
	ParticularExport(void);
	~ParticularExport(void);
	static ParticularExport * Generate(const CString &Title);
protected:
	FILE *ExportFile;

public:
	bool Stop(void);
	bool Write(const CString & String);
};
