//#define _AFXDLL

#include "pch.h"
#include <afx.h>
#include <windows.h>
#include <Wincrypt.h>
#include <iostream>
#include <tchar.h>
#include "Encrypt.h"


#using <System.Security.dll>
#using <System.Xml.dll>

using namespace System;
using namespace System::Security::Cryptography;
using namespace System::Security::Cryptography::X509Certificates;
using namespace System::Security::Cryptography::Xml;
using namespace System::Text;
using namespace System::Xml;


bool Encrypt::Encryption(BYTE* buffer, DWORD& lenData)
{
	HCRYPTPROV m_hCryptoProvider = 0;

	CryptAcquireContext(&m_hCryptoProvider, NULL, NULL/*Default*/, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);

	HCRYPTKEY hKey = 0;
	HCRYPTKEY* phKey = &hKey;
	HCRYPTHASH phHash;
	LPCSTR password = "PPLicensePW";// corresponding password is in the Decrypt::DecryptData
    DWORD error;


	CryptCreateHash(m_hCryptoProvider, CALG_SHA1, 0, 0, &phHash);

	CryptHashData(phHash, (BYTE*)password, (DWORD)_tcslen(password) * sizeof(TCHAR), 0);

	CryptDeriveKey(m_hCryptoProvider, CALG_AES_256, phHash, CRYPT_CREATE_SALT, &hKey);

	int length = sizeof(hKey);

	DWORD lenBuffer = 2 * lenData;

	CryptEncrypt(hKey, NULL, TRUE, 0, buffer, &lenData,  2 * lenData);
    error = GetLastError();
	return !error;
}

bool Encrypt::CreateNewFile(LPCTSTR inputFileName, LPCTSTR outputFileName)
{
	// constructing these file objects doesn't open them
	CFile sourceFile;
	CFile destFile;

	// we'll use a CFileException object to get error information
	//CFileException ex;

	// open the source file for reading
	if (!sourceFile.Open(inputFileName,
		CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary, NULL))
	{
		return false;
	}
	else
	{
		if (!destFile.Open(outputFileName, CFile::modeWrite |
			CFile::shareExclusive | CFile::modeCreate | CFile::typeBinary, NULL))
		{
			return false;
		}

		DWORD fileLength = DWORD(sourceFile.GetLength());
		sourceFile.SeekToBegin();

        std::string fileName = sourceFile.GetFileName();
        auto dotPos = fileName.rfind('.', fileName.length());
        fileName = fileName.substr(0, dotPos);

        DWORD bufflen = DWORD(fileName.length()) + fileLength;
		BYTE* buffer = new BYTE[bufflen*2];

        memcpy(buffer, (LPCTSTR)fileName.data(), fileName.length());

        //BYTE* fileBuffer = new BYTE[fileLength];
		sourceFile.Read(buffer + fileName.length(), fileLength);

       // memcpy(buffer + fileName.length(), fileBuffer, fileLength);

        if (!Encryption(buffer, bufflen))
        {
            return false;
        }

		destFile.Write(buffer, bufflen);
		destFile.Flush();

        delete[] buffer;
		destFile.Close();
		sourceFile.Close();
	}

	return true;
}

void Encrypt::SignXmlFile(String^ FileName, String^ SignedFileName)
{

    // Create a new XML document.
    XmlDocument^ doc = gcnew XmlDocument;

    // Load the passed XML file using its name.
    XmlTextReader^ reader = gcnew XmlTextReader(FileName);
    doc->Load(reader);
    reader->Close();

    // Create a SignedXml object.
    SignedXml^ signedXml = gcnew SignedXml(doc);


    String^ rsaString("<RSAKeyValue><Modulus>vtrQgfSGuAzditm2qFgv+P0T/ewwpT7uSMuYoI7yDlwV88RvfT4SIocDEDIanuEgBE84S1UiSpxyfMdmOq3rOP1m4YSRXYAlpBiLawJOHljnsoVJ/IXFgP1VZXc9pOa57fA/BS5o3z1PBHlpdIwO93o+q6LIEDcH3r9LDwCw35U=</Modulus><Exponent>AQAB</Exponent><P>3CrEuMVJE6WL2gIUAOBMmimQBLcJIFjiY1atrdnmKVK2vNoO7CuZ9y1MsUKjW9HVTYclGKeHH2k6rn4hN9V0uw==</P><Q>3erAidlPSv6LpbEAOi1riIjIKyfmPkCWoIhcjjrDjxsQ+1wRrDqdOcstX9lGFOj+Y8EvzvKhNXzc6wf7kI7f7w==</Q><DP>cx/V//wBS0msSFH/3blrtMz45s5C8mwn7OPdK2QeSwtaTIQFe48NEkfYq3P58cBWNhTt2+d164Hd0nJXsldmkw==</DP><DQ>HI8DbDoNHEeZzR70Oohx2EZSDEu8M7K4L7ycvKkPVbFUBCUbGSEJ7tAY7MbXYAWEB25rKqTK3klNXOnQFBBu/Q==</DQ><InverseQ>xKlbMFAhUab5ow+wl6LraI1Woz4O3nBce0qkHVTTSXymB/6L88C6wXeL4K2i2TWdzYGNMKwRCmeq8DzYbp8Vvg==</InverseQ><D>q7RO3Gvy/AjPFAl09JWKa1a8srjYtwIb/K/225RfHx4nvczfhOzgAeI3lk2Nd50QYfAntFaeaEPCGJKIXYh00p4kx3ypfkL7u1MTZ1cRkCN68o9gpU+1pGMB4WgjIX/wkH3fOk/fifK3tQPLp7fphdxaC4OQ9EqUmwdW3Zha/K0=</D></RSAKeyValue>");

    RSACryptoServiceProvider^ rsa = gcnew RSACryptoServiceProvider();
    rsa->FromXmlString(rsaString);

    // Remove tag signatureKey if it exists
    XmlNodeList^ keyNodeList = doc->GetElementsByTagName("SignatureKey");
    XmlElement^ keyElement = safe_cast<XmlElement^>(keyNodeList->Item(0));
    if (keyElement)
    {
        XmlNode^ parent = keyElement->ParentNode;
        parent->RemoveChild(keyElement);
    }

    //Create XML element to write public key in it
    XmlElement^ xmlKey = doc->CreateElement("SignatureKey");
    xmlKey->InnerText = rsa->ToXmlString(false);
    doc->DocumentElement->AppendChild(xmlKey);

    // Add the key to the SignedXml document. 
    signedXml->SigningKey = rsa;


    // Create a reference to be signed.
    Reference^ reference = gcnew Reference;
    reference->Uri = "";

    // Add an enveloped transformation to the reference.
    XmlDsigEnvelopedSignatureTransform^ env = gcnew XmlDsigEnvelopedSignatureTransform;
    reference->AddTransform(env);

    // Add the reference to the SignedXml object.
    signedXml->AddReference(reference);

    // Compute the signature.
    signedXml->ComputeSignature();

    // Get the XML representation of the signature and save
    // it to an XmlElement object.
    XmlElement^ xmlDigitalSignature = signedXml->GetXml();

    // Append the element to the XML document.
    doc->DocumentElement->AppendChild(doc->ImportNode(xmlDigitalSignature, true));
    if ((doc->FirstChild)->GetType() == XmlDeclaration::typeid)
    {
        doc->RemoveChild(doc->FirstChild);
    }

    // Save the signed XML document to a file specified
    // using the passed string.
    XmlTextWriter^ xmltw = gcnew XmlTextWriter(SignedFileName, gcnew UTF8Encoding(false));
    xmltw->Formatting = Formatting::Indented;
    doc->WriteTo(xmltw);
    xmltw->Close();
}
