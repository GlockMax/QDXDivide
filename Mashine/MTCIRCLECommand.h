// MTCIRCLECommand.h: interface for the MTCIRCLECommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MTCIRCLECOMMAND_H__C91ED9F4_21DD_43CA_A36F_1F348602121D__INCLUDED_)
#define AFX_MTCIRCLECOMMAND_H__C91ED9F4_21DD_43CA_A36F_1F348602121D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MTCommand.h"

class MTCIRCLECommand : public MTCommand  
{
public:
	virtual int GetType();
	MTCIRCLECommand();
	virtual ~MTCIRCLECommand();

};

#endif // !defined(AFX_MTCIRCLECOMMAND_H__C91ED9F4_21DD_43CA_A36F_1F348602121D__INCLUDED_)
