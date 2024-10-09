#pragma once

#include <wincrypt.h>
#include <rpc.h>
#include "NCMVersion.h"
class Security
{
public:
	enum Result
	{
		OK, NoAdapter, Fail
	};
	static Result GetCode(CString* pCode);
	static CString GetDate(int Num);
	static Result GetAnswer(CString code, CString* pAnswer);
	static Result CheckAnswer(CString answer, bool* pCheckResult);
	static bool _GetHash(unsigned char* pMac, unsigned char* pHash, int Length = MAC_SIZE);
	static bool _GetMac(unsigned char* pMac);


private:
	
	static const int MAC_SIZE = 6;
	static const int MD5_HASH_SIZE = 16;
	static const int DATE_SIZE = 4;
	static const int WEEK_SECONDS = 7*24*60*60;

};
