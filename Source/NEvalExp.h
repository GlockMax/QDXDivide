#pragma once

#include "MNEvalExp.h"

class NCUnit;
class NEvalExp  : public MNEvalExp
{
// Construction / destruction
public:
	NEvalExp( NCUnit* caller, bool UseIntegerArithm = false, bool ConsecutiveCalc = false);
	~NEvalExp();

// Functionality
public:
	bool Evaluate( const CString& exp, double* res );
	
// Internal functions
protected:
	bool PlaceOperand( const Token& t ) override;
	bool GetIdentifier( CString& name ) override;
	bool ApplyFunction( void ) override;

// Data
protected:
	NCUnit* caller;
};
