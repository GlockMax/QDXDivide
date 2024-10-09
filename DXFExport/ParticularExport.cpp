#include "stdafx.h"
#include "ParticularExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

ParticularExport::ParticularExport(void)
{
	ExportFile = NULL;
}

ParticularExport::~ParticularExport(void)
{
	Stop();
}

ParticularExport * ParticularExport::Generate(const CString &Title)
{
	CString filename, filter, ext = "csv";
	filter = "*." + ext +"|*." + ext + "|";
	CFileDialog fd(FALSE, ext, filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	if(fd.DoModal() == IDOK)
		filename = fd.GetPathName();
	else 
		return NULL;
	FILE *EFile;
	if( (EFile = fopen(filename, "w")) == NULL )
		return NULL;
	ParticularExport *pPE = new ParticularExport;
	if(!pPE)
	{
		fclose(EFile);
		return NULL;
	}
	pPE->ExportFile = EFile;
	if(!pPE->Write(Title))
	{
		fclose(EFile);
		delete pPE;
		return NULL;
	}
	return pPE;
}

bool ParticularExport::Stop(void)
{
	if(ExportFile)
		fclose(ExportFile);
	ExportFile = NULL;
	return true;
}

bool ParticularExport::Write(const CString & String)
{
	if(ExportFile)
		return fprintf(ExportFile, String + "\n", "%s") >= 0;
	return false;
}
