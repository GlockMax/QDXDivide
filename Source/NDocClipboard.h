#pragma once

#include "NCMComData.h"
#include "NMapStringToNTool.h"

class NDocClipboard
{
	friend class CNCMDoc;
public:
	NDocClipboard(void);
	~NDocClipboard(void);
protected:
	NMapStringToNTool NToolList;
	CString BaseDocPath;
	NCMApplicationType AppType;

};
