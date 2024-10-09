#include "stdafx.h"
#include "NDocClipboard.h"

NDocClipboard::NDocClipboard(void)
{
	AppType = AT_UNDEF;
}

NDocClipboard::~NDocClipboard(void)
{
	NToolList.FullRemoveAll();
}
