// NEvalExp.cpp: implementation of the NEvalExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCUnit.h"
#include "math.h"
#include "NEvalExp.h"

NEvalExp::NEvalExp( NCUnit* caller, bool UseIntegerArithm /*= false*/, bool iConsecutiveCalc /*= false*/ ) 
	: MNEvalExp(UseIntegerArithm /*= false*/, iConsecutiveCalc /*= false*/)
{
	ASSERT( caller );
	this->caller = caller;
}

NEvalExp::~NEvalExp()
{
}

bool NEvalExp::Evaluate( const CString& exp, double* res )
{
	expression = openBracketChar + exp + closeBracketChar;
	if (caller != nullptr)
	{
		char RegSymbol = caller->GetRegSymbol();
		expression.Replace(RegSymbol, FUNC_NAMES.GetAt(f_reg));
	}
	return Eval(res);
}

bool NEvalExp::PlaceOperand( const Token &t )
{
// If token is constant push it
// Else get value of the variable and also push
	if( !MNEvalExp::PlaceOperand(t))
	{
		if(caller == nullptr)
		{
			error = Syn_BadVariable;
			return false;
		}
		double val;
		bool integer;
		if( !caller->GetParamValue( t.svalue, &val, integer ) )
		{
			error = Syn_BadVariable;
			return false;
		}
		integer = integer && useIntegerArithm;
		Token tv;
		tv.type = integer ? Token::ConstantInt : Token::Constant;
		tv.dvalue = val;
		operands<<tv;
	}
	return true;
}

bool NEvalExp::ApplyFunction( void )
{
	Token func;
	operators>>func;
	Token arg;
	operands>>arg;
	
	if( arg.type == Token::Empty )
	{
		error = Syn_UnexpectedFunction;
		return false;
	}
	double res = 0.;
	bool Done = GetFuncResult(func, arg, res);

	if (error != NoErrors)
		return false;

	if(!Done && func.svalue == CString(FUNC_NAMES.GetAt(f_reg)))
	{
		if (caller != nullptr)
			caller->GetRegValue(int(arg.dvalue + 0.5), &res);
	}
	if(res != res)
	{
		error = Math_Domain;
		return false;
	}
	Token tr;
	tr.type = Token::Constant;
	tr.dvalue = res;
	operands<<tr;
	return true;
}

bool NEvalExp::GetIdentifier( CString& value )
{
	if( isdigit( expression[0] ) )
		return false;
//	sscanf( expression, "$[^+-/*().\x0]s", buf );
	// changed 14.02.06 for function calls without brackets
	value = expression.SpanIncluding(FUNC_NAMES);
	if( value.GetLength() > 1)
		return false;
	if (value.GetLength() < 1)
	{
		if(caller == nullptr)
			value = expression.SpanExcluding(_T("-+*/^$<>=!?&|:'") + CString(NEvalExp::OpenBC()) + CString(NEvalExp::CloseBC()) + _T("\x0\t"));
		else
			value = expression.SpanExcluding(caller->OpCharSet + _T("\x0\t"));
	}
	//END changed 14.02.06 for function calls without brackets
	if( value.IsEmpty() )
		return false;
	expression = expression.Right( expression.GetLength() - value.GetLength() );
	return true;
}
