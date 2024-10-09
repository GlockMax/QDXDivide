#pragma once

#include <afx.h>
#include <windows.h>
#include <Wincrypt.h>
#include <string>
#include "PPLicense_API.h"

class PPLicense_API Decrypt
{
private:
	static bool DecryptData(BYTE* buffer, DWORD& lenData);
	static bool VerifySignature(LPCTSTR licenseFileName);
	static bool FindFileInLicense(LPCTSTR licenseFileName, LPCTSTR fileName);
	static bool FindClientIDInLicense(LPCTSTR licenseFileName, LPCTSTR ClientID);
	static bool compareNames(const std::string& str1, const std::string& str2);

public:
	enum  decryptionError{NONE, LICENSE_FILE_NOT_FOUND, POSTPROCESSOR_FILE_NOT_FOUND, INVALID_CLIENT_ID_IN_LICENSE, NO_ACCESS_TO_POSTPROCESSOR_FILE, INVALID_SIGNATURE, FILE_READ_FAILED, DECRYPTION_FAILED, INVALID_POSTPOCESSOR_FILE};
	//decription without signature verification
	static decryptionError DecryptPPFile(LPCTSTR encryptedPPFile, CMemFile& decryptedMemFile);
	//decription with signature verification
	static decryptionError DecryptPPFile(LPCTSTR licenseFile, LPCTSTR nameSource, LPCTSTR ClientID, CMemFile& nameDest);
};

