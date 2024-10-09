// IconHandler.cpp : Implementation of CIconHandler

#include "stdafx.h"
#include "strsafe.h"
#include "NCMComData.h"
#include "IconHandler.h"


// CIconHandler
STDMETHODIMP CIconHandler::GetIconLocation(
    UINT   uFlags,
    LPTSTR szIconFile,
    UINT   cchMax,
    int* piIndex,
    UINT* pwFlags)
{
    HANDLE hFile = CreateFile(m_szFilename, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile)
        return S_FALSE;    // tell the shell to use a default icon

    CFile f(hFile);
    CArchive ar(&f, CArchive::load);
    CStringA str1, str2, str3;
    NCMApplicationType Type = AT_UNDEF;
    try
    {
        SerializeElements(ar, &str1, 1);
        SerializeElements(ar, &str2, 1);
        SerializeElements(ar, &str3, 1);
        SerializeElements(ar, &Type, 1);
    }
    catch (...)
    {
        ar.Close();
        f.Close();
        return S_FALSE;    // tell the shell to use a default icon
    }
    ar.Close();
    f.Close();
    if(str1 != CStringA(_T("NCM File.")) || str2.Left(4) != CStringA(_T(" Ver")))
        return S_FALSE;    // tell the shell to use a default icon

    TCHAR szModulePath[MAX_PATH];
    GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModulePath, MAX_PATH);
    if (GetLastError() != NO_ERROR)
        return S_FALSE;    // tell the shell to use a default icon
    StringCchCopy(szIconFile, cchMax, szModulePath);

    *piIndex = 0;
    //we set pwFlags to 0 to get the default behavior from Explorer.
    //This means that it checks its icon cache to see if the icon specified by szIconFile / piIndex is in the cache.If it is, 
    //then IExtractIcon::Extract() will not be called.We then return S_OK to indicate that GetIconLocation() succeeded. 
     *pwFlags = 0;
    
    if(atof(str3) < 5.3)
        return S_OK;

    switch (Type)
    {
    case AT_MILL:
        *piIndex = 1;
        break;
    case AT_TURN:
        *piIndex = 2;
        break;
    case AT_MILL_TURN:
        *piIndex = 3;
        break;
    case AT_PUNCH:
        *piIndex = 0;
        break;
    case AT_WELD:
        *piIndex = 0;
        break;
    case AT_UNDEF:
    default:
        *piIndex = 0;
        break;
    }
    return S_OK;
}


