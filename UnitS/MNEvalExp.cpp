// MNEvalExp.cpp: implementation of the MNEvalExp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "MNEvalExp.h"

char MNEvalExp::openBracketChar = '[';
char MNEvalExp::closeBracketChar = ']';

MNEvalExp::MNEvalExp( bool UseIntegerArithm /*= false*/, bool iConsecutiveCalc /*= false*/ )
{
	error = NoErrors;
	ConsecutiveCalc = iConsecutiveCalc;
	useIntegerArithm = UseIntegerArithm;
}

MNEvalExp::~MNEvalExp()
{
}

bool MNEvalExp::Eval(double* res)
{
	error = NoErrors;
	if (expression.GetLength() <= 2)
	{
		error = Syn_EmptyExp;
		return false;
	}
	KillUnary();

	Token token;
	while (GetToken(token))
	{
		if (token.type == Token::Empty)
		{
			error = Internal;
			return false;
		}

		if (token.type == Token::ConstantInt || token.type == Token::Constant || token.type == Token::Variable)
		{
			if (!PlaceOperand(token))
				return false;
		}
		else
		{
			if (!PlaceOperator(token))
				return false;
		}
	}
	if (!operators.IsEmpty())
	{
		error = Syn_UnexpectedEnd;
		return false;
	}
	if (operands.GetCount() != 1)
	{
		error = Syn_UnexpectedEnd;
		return false;
	}
	Token result;
	operands >> result;
	*res = result.dvalue;
	return true;

}
bool MNEvalExp::Evaluate( const CString& exp, double* res )
{
	expression = openBracketChar + exp + closeBracketChar;
	return Eval(res);
}

bool MNEvalExp::GetErrorInfo( EvalError& errorInfo )
{
	if( error == NoErrors )
		return false;
	errorInfo = error;
	return true;
}

bool MNEvalExp::GetToken( Token& token )
{
	CString svalue;

	if( expression.IsEmpty() )
	{
		token.type = Token::Empty;
		return false;
	}
	char e0 = expression[0];	
	if( e0 == openBracketChar)
	{
		token.type = Token::OpenBracket;
		token.svalue = CString( e0 );
		expression = expression.Right( expression.GetLength() - 1 );
		return true;
	}
	if( e0 == closeBracketChar)
	{
		token.type = Token::CloseBracket;
		token.svalue = CString( e0 );
		expression = expression.Right( expression.GetLength() - 1 );
		return true;
	}
// Add operator here
// ”нарный минус обрабатываетс€ отдельно.
	if( e0 == '+' || e0 == '*' || e0 == '/' || e0 == '>' || e0 == '<' || e0 == '=' || e0 == '&' || e0 == '|' || e0 == '?' || e0 == '!' || e0 == ':' || e0 == '^' || e0 == '$' || e0 == '\'')
	{
		token.type = Token::Operator;
		token.svalue = CString( e0 );
		expression = expression.Right( expression.GetLength() - 1 );
		return true;
	}
	if(e0 == '-')
	{
		if(token.type == Token::Operator && token.svalue != ')')
		{
			token.svalue = CString('~'); // ”нарный минус.
		}
		else
		{
			token.svalue = CString('-'); // ќбычный минус.
		}
		token.type = Token::Operator;
		expression = expression.Right( expression.GetLength() - 1 );
		return true;
	}
	if( GetConstant( token ) ) 
	{
		return true;
	}
	if( !GetIdentifier( svalue ) )
	{
		error = Syn_UnhandledError;
		token.type = Token::Empty;
		return false;
	}
	if( FUNC_NAMES.Find(svalue) >= 0 )// changed 14.02.06 for function calls without brackets
	{
		token.type = Token::Function;
		token.svalue = svalue;
//		expression = expression.Right( expression.GetLength() - 1 );
		return true;
	}
	else
	{
		token.type = Token::Variable;
		token.svalue = svalue;
		return true;
	}
}

bool MNEvalExp::PlaceOperand( const Token &t )
{
// If token is constant push it
// Else get value of the variable and also push
	if (t.type == Token::ConstantInt || t.type == Token::Constant)
	{
		operands << t;
		return true;
	}
	return false;
}

bool MNEvalExp::PlaceOperator( const Token &t )
{
// Copy t to tc because it is not possible to work with const object
	Token tc = t;
	SetPriority( tc );

	if( operators.IsEmpty() && t.type == Token::CloseBracket )
	{
		error = Syn_UnexpectedCB;
		return false;
	}
	
// Guaranteed by used algorithm that stack operators contains only 
// Operators, Functions and OpenBrackets

	if( tc.type == Token::OpenBracket )
	{
		operators<<tc;
		return true;
	}
	Token prev;
	operators>>prev;
	if( prev.type == Token::Function )
	{
		operators<<prev;
		if( !ApplyFunction() )
			return false;
		operators>>prev;
	}

	if( tc.type == Token::OpenBracket || tc.type == Token::Function )
	{
		operators<<prev;
		operators<<tc;
		return true;
	}


	while( prev.pvalue >= tc.pvalue && prev.type != Token::Empty )
	{
		if( prev.type == Token::Operator )
		{
			operators<<prev;
			if( !ApplyOperator() )
				return false;
		}
		operators>>prev;
	}
// tc.type can be only Operator or CloseBracket
	if( tc.type == Token::Operator )
	{
		if( prev.type != Token::Empty )
			operators<<prev;
		operators<<tc;
		return true;
	}

// Now tc.type can be only CloseBracket
// prev.type can be either OpenBracket either Function or Operator

	if( prev.type == Token::OpenBracket || prev.type == Token::Empty )
// Kill both ( and ). Expression between them has been evaluated 
// and stored in operands
		return true;

	if( prev.type == Token::Operator )
	{
		error = Syn_UnexpectedCB;
		return false;
	}
	error = Syn_UnhandledError;
	return false;
}

void MNEvalExp::SetPriority( Token &t )
{
	if( t.type == Token::Empty )
	{
		t.pvalue = 0;
		return;
	}
// Add operator here
	if(ConsecutiveCalc)
	{
		if(t.svalue[0] == closeBracketChar)
			t.pvalue = 2;
		else if (t.svalue[0] == openBracketChar)
			t.pvalue = 1;
		else
		{
			switch (t.svalue[0])
			{
			case '<':
			case '?':
			case '>':
			case '!':
			case ':':
			case '=': { t.pvalue = 4; break; }
			case '+':
			case '-':
			case '^':
			case '$':
			case '\'':
			case '~':
			case '*':
			case '/': { t.pvalue = 6; break; }
			case '|':
			case '&': { t.pvalue = 3; break; }
			default: { t.pvalue = 1; break; }
			}
		}
	}
	else
	{
		if (t.svalue[0] == closeBracketChar)
			t.pvalue = 2;
		else if (t.svalue[0] == openBracketChar)
			t.pvalue = 1;
		else
		{
			switch (t.svalue[0])
			{
			case '<':
			case '?':
			case '>':
			case '!':
			case ':':
			case '=': { t.pvalue = 4; break; }
			case '+':
			case '-': { t.pvalue = 5; break; }
			case '^':
			case '$':
			case '\'':
			case '~':
			case '*':
			case '/': { t.pvalue = 6; break; }
			case '|':
			case '&': { t.pvalue = 3; break; }
			default: { t.pvalue = 1; break; }
			}
		}
	}
}

bool MNEvalExp::ApplyOperator( void )
{
	Token opr;
	operators>>opr;

// ”нарный минус обрабатываем отдельно.
	if(opr.type == Token::Operator && opr.svalue[0] == '~')
	{
		Token operand;
		operands>>operand;
		operand.dvalue = -operand.dvalue;
		operands<<operand;
		return true;
	}

	Token opd1, opd2;
	double opdl, opdr, res = 0.;

	operands>>opd1;
	operands>>opd2;


	if( opd1.type == Token::Empty || opd2.type == Token::Empty)
	{
		error = Syn_UnexpectedOperator;
		return false;
	}

	opdr = opd1.dvalue;
	opdl = opd2.dvalue;
	
// Add operator here
	switch( opr.svalue[0] )
	{
		case '+': { res = opdl + opdr; break; }
		case '-': { res = opdl - opdr; break; }
		case '*': { res = opdl * opdr; break; }
		case '<': { res = opdl < opdr; break; }
		case '?': { res = opdl <= opdr; break; }
		case '>': { res = opdl > opdr; break; }
		case '!': { res = opdl >= opdr; break; }
		case '=': { res = opdl == opdr; break; }
		case ':': { res = opdl != opdr; break; }
		case '&': { res = opdl && opdr; break; }
		case '|': { res = opdl || opdr; break; }
		case '$': { res = int(opdl) % int(opdr); break; }
		case '\'':{ res = int( opdl / opdr); break;}
		case '/':
		{
			if( fabs( opdr ) < 1e-308 )
			{
				error = Math_DivisionByZero;
				return false;
			}
			res = opdl / opdr; 
			break; 
		}
		case '^': { res = pow(opdl, opdr); break; }
	}

	Token tr;
	tr.type = (useIntegerArithm && opd1.type == Token::ConstantInt && opd2.type == Token::ConstantInt) ? Token::ConstantInt : Token::Constant;
	tr.dvalue = (tr.type == Token::ConstantInt) ? int(res) : res;
	operands<<tr;
	return true;
}

bool MNEvalExp::ApplyFunction( void )
{
	Token func;
	operators >> func;
	Token arg;
	operands >> arg;

	if (arg.type == Token::Empty)
	{
		error = Syn_UnexpectedFunction;
		return false;
	}
	double res = 0.;
	bool Done = GetFuncResult(func, arg, res);

	if (error != NoErrors)
		return false;

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

bool MNEvalExp::GetConstant( Token& res )
{
	double value;
	if( !sscanf( expression, "%lf", &value ) )
		return false;
	res.dvalue = value;
	res.type = Token::Constant;

	int pos = 0;
	int len = expression.GetLength();
// Read mantissa

//	Read sign
	if( expression[pos] == '+' || expression[pos] == '-' )
		pos++;

//	Read integer part
	for( ; pos < len && isdigit( expression[pos] ); pos++ );
	if( pos == len )
	{
		expression = expression.Right( len - pos );
		return true;
	}
//	Read decimal point
	if (expression[pos] == '.')
	{
		pos++;
		// Read fraction part
		for (; pos < len && isdigit(expression[pos]); pos++);
	}
	else
		res.type = useIntegerArithm ? Token::ConstantInt : Token::Constant;
	if( pos == len )
	{
		expression = expression.Right( len - pos );
		return true;
	}
	
// Read exponent
	if( expression[pos] != 'e' && expression[pos] != 'E' )
	{
		expression = expression.Right( len - pos );
		return true;
	}
	res.type = Token::Constant;
	pos++;
	
//	Read sign
	if( expression[pos] == '+' || expression[pos] == '-' )
		pos++;
//	Read integer part
	for( ; pos < len && isdigit( expression[pos] ); pos++ );
	expression = expression.Right( len - pos );
	return true;
}

bool MNEvalExp::GetIdentifier( CString& value )
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
			value = expression.SpanExcluding(_T("-+*/^$<>=!?&|:'") + CString(MNEvalExp::OpenBC()) + CString(MNEvalExp::CloseBC()) + _T("\x0\t"));
	}
	//END changed 14.02.06 for function calls without brackets
	if( value.IsEmpty() )
		return false;
	expression = expression.Right( expression.GetLength() - value.GetLength() );
	return true;
}

void MNEvalExp::KillUnary()
{
	for( int i = 1; i < expression.GetLength(); i++ )
		if( expression[i] == '+' || expression[i] == '-' )
			if( expression[i-1] == openBracketChar)
				expression.Insert( i, '0' );
}

bool MNEvalExp::GetFuncResult(const Token& func, const Token& arg, double& res)
{
	// List of defined functions...add here or in NEvalExp::ApplyFunction

	const double coef = 3.141592655358979 / 180.;
	res = 0.;
	if (func.svalue == CString(FUNC_NAMES.GetAt(f_sin)))
		res = sin(arg.dvalue * coef);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_cos)))
		res = cos(arg.dvalue * coef);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_sqrt)))
	{
		if (arg.dvalue < 0.0)
		{
			error = Math_SQRTLTZ;
			return false;
		}
		res = sqrt(arg.dvalue);
	}
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_abs)))
		res = fabs(arg.dvalue);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_round)))
		res = int(arg.dvalue + 0.5);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_roundup)))
		res = (int(arg.dvalue) == arg.dvalue) ? arg.dvalue : int(arg.dvalue + 1.);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_rounddown)))
		res = int(arg.dvalue);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_tan)))
		res = tan(arg.dvalue * coef);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_atan)))
		res = atan(arg.dvalue) / coef;
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_asin)))
	{
		if (arg.dvalue < -1.0 || arg.dvalue > 1.0)
		{
			error = Math_Domain;
			return false;
		}
		res = asin(arg.dvalue) / coef;
	}
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_acos)))
	{
		if (arg.dvalue < -1.0 || arg.dvalue > 1.0)
		{
			error = Math_Domain;
			return false;
		}
		res = acos(arg.dvalue) / coef;
	}
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_log)))
	{
		if (arg.dvalue < 0.0)
		{
			error = Math_Domain;
			return false;
		}
		res = log(arg.dvalue);
	}
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_exp)))
		res = exp(arg.dvalue);
	else if (func.svalue == CString(FUNC_NAMES.GetAt(f_square)))
		res = arg.dvalue * arg.dvalue;
	else
		return false;
	return true;
}
// Added because of incorrect results of original version with cyrillic
bool MNEvalExp::isdigit( char c )
{
	return	c == '0' || c == '1' || c == '2' || c == '3' || c == '4' ||
			c == '5' ||	c == '6' || c == '7' || c == '8' || c == '9';
}