#include "StdAfx.h"
#include "atlenc.h"
#include ".\security.h"

CString Security::GetDate(int Num)
{
	// Вычисляем номер пятинедельного диапазона в котором текущая неделя имеет номер Num
	// Нумерация начинается с нуля
	time_t curtime;
	time(&curtime);
	time_t week = curtime / WEEK_SECONDS;
	week -= Num;
	CString str;
	str.Format("%d", week);
	return str.Left(4);
}
Security::Result Security::GetCode(CString* pCode)
{
	// Получаем MAC-адрес.
	unsigned char mac[MAC_SIZE];
	if(!_GetMac(mac)) return NoAdapter;

	// Вычисляем хэш MD5 от MAC-адреса.
	unsigned char hash[MD5_HASH_SIZE];
	if(!_GetHash(mac, hash)) return Fail;

	// Переводим в base64.
	char buffer[1024];
	DWORD length = 1024;
//	::CryptBinaryToString(hash, MD5_HASH_SIZE, CRYPT_STRING_BASE64, buffer, &length);
	int len = 1024;
	Base64Encode(hash,MD5_HASH_SIZE, buffer, &len); 

	// Всё :)
	*pCode = CString(buffer).Left(8);

	return OK;
}

Security::Result Security::GetAnswer(CString code, CString* pAnswer)
{
	Security::Result result = Fail;

	// Открываем криптографический контекст.
	HCRYPTPROV hCryptProv;
	if(!::CryptAcquireContext(&hCryptProv, 0, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) goto exit1;

	// Создаём хэш.
	HCRYPTHASH hHash;
	if(!::CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash)) goto exit2;

	// Переводим код в двоичный формат и вычисляем хэш.
	BYTE bytes[1024];
	DWORD length = 1024;
//	if(!::CryptStringToBinary(code, code.GetLength(), CRYPT_STRING_BASE64, bytes, &length, &skip, &flags)) goto exit3;
	int len = 1024;
	if(!Base64Decode(code, code.GetLength(), bytes, &len)) goto exit3;
	length = len;

	if(!::CryptHashData(hHash, bytes, length, 0)) goto exit4;

	// Извлекаем хэш.
	BYTE hash[MD5_HASH_SIZE];
	length = MD5_HASH_SIZE;
	if(!::CryptGetHashParam(hHash, HP_HASHVAL, hash, &length, 0)) goto exit5;

	// Переводим в строку.
	char buffer[1024];
	length = 1024;
	len = 1024;
//	int len;
	if(!Base64Encode(hash,MD5_HASH_SIZE, buffer, &len)) goto exit6;
//	if(!::CryptBinaryToString(hash, MD5_HASH_SIZE, CRYPT_STRING_BASE64, buffer, &length)) goto exit6;

	*pAnswer = CString(buffer).Left(8);

	result = OK;

exit6:
exit5:
exit4:
exit3: ::CryptDestroyHash(hHash);
exit2: ::CryptReleaseContext(hCryptProv, 0);
exit1: return result;
}

Security::Result Security::CheckAnswer(CString answer, bool* pCheckResult)
{
	Security::Result result = Fail;
	*pCheckResult = false;

	CString code, ans;
	CString Conf = answer.Right(5);
	answer = answer.Left(answer.GetLength() - 5);

	if(GetCode(&code) != OK) goto exit1;
	code = code + NCMVersion + Conf;

	if(GetAnswer(code, &ans) != OK) goto exit2;
	
	if(ans == answer)
	{
		*pCheckResult = true;
		return OK;
	}
	for(int i = 0; i < 5 ; ++i)
	{
		if(GetAnswer(code + GetDate(i), &ans) != OK) goto exit2;
		
		if(ans == answer)
		{
			*pCheckResult = true;
			break;
		}
	}
	result = OK;

exit1:
exit2:
	return result;
}


bool Security::_GetMac(unsigned char* pMac)
{
	UUID uuid;
	
	RPC_STATUS res = UuidCreateSequential(&uuid);
	if(res != RPC_S_OK) return false;

	for(int i = 0; i < MAC_SIZE; ++i) pMac[i] = uuid.Data4[i+2];
	
	return true;
}

bool Security::_GetHash(unsigned char* pMac, unsigned char* pHash, int Length)
{
	bool result = false;

	HCRYPTPROV hCryptProv;
	HCRYPTHASH hHash;

	if(!::CryptAcquireContext(&hCryptProv, 0, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) goto exit1;

	if(!::CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash)) goto exit2;

	if(!::CryptHashData(hHash, pMac, Length, 0)) goto exit3;

	DWORD length = MD5_HASH_SIZE;
	if(!::CryptGetHashParam(hHash, HP_HASHVAL, pHash, &length, 0)) goto exit4;

	result = true;

exit4:
exit3: ::CryptDestroyHash(hHash);
exit2: ::CryptReleaseContext(hCryptProv, 0);
exit1: return result;
}
