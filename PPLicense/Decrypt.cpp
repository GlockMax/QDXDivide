#include "pch.h"
#include "targetver.h"
#include <filesystem>
#include "PPLicense_api.h"

#using <System.Security.dll>
#using <System.Xml.dll>

using namespace System::Security::Cryptography;
using namespace System::Security::Cryptography::Xml;
using namespace System::Xml;

#include "Decrypt.h"

bool Decrypt::DecryptData(BYTE* buffer, DWORD& lenData)
{
	HCRYPTPROV m_hCryptoProvider = 0;
	CryptAcquireContext(&m_hCryptoProvider, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
	HCRYPTKEY hKey = 0;
	HCRYPTKEY* phKey = &hKey;
	HCRYPTHASH hHash;
	LPCSTR password = "PPLicensePW";// corresponding password is in the Encrypt::Encryption
	DWORD error = 0;
	CryptCreateHash(m_hCryptoProvider, CALG_SHA1, 0, 0, &hHash);
	CryptHashData(hHash, (BYTE*)password, (DWORD)_tcslen(password) * sizeof(TCHAR), 0);
	CryptDeriveKey(m_hCryptoProvider, CALG_AES_256, hHash, CRYPT_CREATE_SALT, &hKey);
	CryptDecrypt(hKey, NULL, TRUE, 0, buffer, &lenData);
	error = GetLastError();
	return !error;
}

bool Decrypt::VerifySignature(LPCTSTR licenseFileName)
{
	// Create a new XML document.
	XmlDocument^ xmlDocument = gcnew XmlDocument;

	// Load the passed XML file into the document. 
	xmlDocument->Load(gcnew System::String(licenseFileName));

	// Create a new SignedXml object and pass it the XML document class.
	SignedXml^ signedXml = gcnew SignedXml(xmlDocument);

	// Find the "Signature" node and create a new XmlNodeList object.
	XmlNodeList^ nodeList = xmlDocument->GetElementsByTagName("Signature");

	if (!nodeList->Item(0))
	{
		return false;	//there is no tag "Signature" in license file
	}

	// Load the signature node.
	signedXml->LoadXml(safe_cast<XmlElement^>(nodeList->Item(0)));

	// Load the public key node.
	XmlNodeList^ keyNodeList = xmlDocument->GetElementsByTagName("SignatureKey");
	XmlElement^ keyElement = safe_cast<XmlElement^>(keyNodeList->Item(0));
	
	if (!keyElement)
	{
		return false;	//there is no tag "SignatureKey" in license file
	}

	// Compare public keys
	System::String^ keyFromLicense = keyElement->InnerText;
	System::String^ publicKey("<RSAKeyValue><Modulus>vtrQgfSGuAzditm2qFgv+P0T/ewwpT7uSMuYoI7yDlwV88RvfT4SIocDEDIanuEgBE84S1UiSpxyfMdmOq3rOP1m4YSRXYAlpBiLawJOHljnsoVJ/IXFgP1VZXc9pOa57fA/BS5o3z1PBHlpdIwO93o+q6LIEDcH3r9LDwCw35U=</Modulus><Exponent>AQAB</Exponent></RSAKeyValue>");
	if (keyFromLicense != publicKey)
	{
		return false;
	}

	// Create RSA object with public key
	RSACryptoServiceProvider^ rsa = gcnew RSACryptoServiceProvider();
	rsa->FromXmlString(publicKey);

	// Check the signature and return the result.
	return signedXml->CheckSignature(rsa);
}

bool Decrypt::FindFileInLicense(LPCTSTR licenseFileName, LPCTSTR fileName)
{
	System::String^ strLicenseFileName = gcnew System::String(licenseFileName);
	std::filesystem::path ppPath(fileName);

	XmlDocument^ doc = gcnew XmlDocument;

	// Load the passed XML file using its name.
	doc->Load(gcnew XmlTextReader(strLicenseFileName));


	//Get names of postprocessor files from license
	XmlNodeList^ ppFileList = doc->GetElementsByTagName("Files");
	XmlElement^ ppFileElement = safe_cast<XmlElement^>(ppFileList->Item(0));
	if (!ppFileElement)		//there is no tag "Files" in license file
	{
		return false;
	}

	//find postprocessor file in list
	System::String^ ppFileName = gcnew System::String(ppPath.filename().string().c_str());
	for (int i = 0; i < ppFileElement->ChildNodes->Count; i++)
	{
		if ((ppFileElement->ChildNodes[i]->InnerText) == ppFileName)
		{
			return true;
		}
	}
	return false;
}

bool Decrypt::FindClientIDInLicense(LPCTSTR licenseFileName, LPCTSTR ClientID)
{
	XmlDocument^ doc = gcnew XmlDocument;

	// Load the passed XML file using its name.
	doc->Load(gcnew XmlTextReader(gcnew System::String(licenseFileName)));

	// Get Client_ID from license
	XmlNodeList^ ClientIDList = doc->GetElementsByTagName("ClientID");
	XmlElement^ ClientIDElement = safe_cast<XmlElement^>(ClientIDList->Item(0));
	if (ClientIDElement)
	{
		//Check Client_ID
		return (ClientIDElement->InnerText == gcnew System::String(ClientID));
	}
	return false;
}

Decrypt::decryptionError Decrypt::DecryptPPFile(LPCTSTR licenseFile, LPCTSTR encryptedPPFile, LPCTSTR ClientID, CMemFile& decryptedMemFile)
{
	if (!System::IO::File::Exists(gcnew System::String(licenseFile)))
	{
		return LICENSE_FILE_NOT_FOUND;
	}

	if (!System::IO::File::Exists(gcnew System::String(encryptedPPFile)))
	{
		return POSTPROCESSOR_FILE_NOT_FOUND;
	}

	if (!FindClientIDInLicense(licenseFile, ClientID))
	{
		return INVALID_CLIENT_ID_IN_LICENSE;
	}

	if (!FindFileInLicense(licenseFile, encryptedPPFile))
	{
		return NO_ACCESS_TO_POSTPROCESSOR_FILE;
	}

	if (!VerifySignature(licenseFile))
	{
		return INVALID_SIGNATURE;
	}

	return DecryptPPFile(encryptedPPFile, decryptedMemFile);
}

Decrypt::decryptionError Decrypt::DecryptPPFile(LPCTSTR encryptedPPFile, CMemFile& decryptedMemFile)
{
	//Open decrypted postprocessor file
	CFile encryptedFile;
	if (!encryptedFile.Open(encryptedPPFile, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary, NULL))
	{
		return FILE_READ_FAILED;
	}

	//read bytes from file
	DWORD fileLength = static_cast<DWORD>(encryptedFile.GetLength());
	BYTE* buffer = new BYTE[fileLength + 1];
	encryptedFile.Read(buffer, fileLength);

	//Decrypt bytes from file
	if (!DecryptData(buffer, fileLength))
	{
		return DECRYPTION_FAILED;
	}

	//Get file name
	std::string fileName(encryptedPPFile);
	auto i = fileName.rfind('.', fileName.length());
	fileName = fileName.substr(0, i);
	i = fileName.rfind('\\', fileName.length());
	fileName = fileName.erase(0, i + 1);
	std::string bufferFileName = fileName;

	//Get file name from buffer
	UINT nameLength = static_cast<UINT>(fileName.length());
	memcpy((char*)bufferFileName.data(), buffer, nameLength);
	//Check file name
	if(!compareNames(fileName, bufferFileName))
		return INVALID_POSTPOCESSOR_FILE;

	buffer[fileLength + 1] = '\0';

	//write bytes from buffer to MemFile (except file name)
	decryptedMemFile.Write(buffer + nameLength, fileLength - nameLength + 1);
	decryptedMemFile.Flush();
	encryptedFile.Close();
	delete[] buffer;
	return NONE;
}

bool Decrypt::compareNames(const std::string& str1, const std::string& str2)
{
	if (str1.length() != str2.length())
	{
		return false;
	}
	for (int k = 0; k < str1.length(); ++k)
	{
		if (tolower(str1[k]) != tolower(str2[k]))
			return false;
	}
	return true;
}