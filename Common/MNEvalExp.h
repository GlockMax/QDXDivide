#pragma once
#include "UNITS_API.h"
typedef enum
{
	Math_DivisionByZero,	// Attempt to divide by zero
	Math_SQRTLTZ,			// Sqrt's argument less than zero
	Math_Overflow,			// Overflow ( + or * )
	Math_Domain,			// Wrong domain
	Syn_EmptyExp,			// Expression to evaluate is empty
	Syn_UnexpectedEnd,		// Unexpected end of expression
	Syn_UnexpectedCB,		// Unexpected closing bracket
	Syn_UnexpectedOperator,	// Unexpected operator
	Syn_UnexpectedFunction,	// Unexpected function
	Syn_BadVariable,		// GetRegValue returned false
	Syn_UnhandledError,		// All other errors
	Internal,				// Internal error. Contact manufacturer
	NoErrors
} EvalError;

const int FUNC_NUMBER = 15;
//CHINA const CString FUNC_NAMES("скцматдрин");
const CString FUNC_NAMES = _T("jkscravtqefilm\30");
const CString FUNC_TITLES[FUNC_NUMBER] = { "ASIN", "ACOS", "SIN", "COS", "ROUND", "ABS", "ATAN", "TAN", "SQRT", "REG", "FUP", "FIX", "LN", "EXP", "POT" };

class UNITS_API MNEvalExp
{
protected:
	enum Functions
	{
		f_asin = 0,
		f_acos = 1,
		f_sin = 2,
		f_cos = 3,
		f_round = 4,
		f_abs = 5,
		f_atan = 6,
		f_tan = 7,
		f_sqrt = 8,
		f_reg = 9,
		f_roundup = 10,
		f_rounddown = 11,
		f_log = 12,
		f_exp = 13,
		f_square = 14
	};
	// Construction / destruction
public:
	MNEvalExp( bool UseIntegerArithm = false, bool ConsecutiveCalc = false);
	~MNEvalExp();
	// Functionality
public:
	bool GetErrorInfo( EvalError& errInfo );
	bool Evaluate( const CString& exp, double* res );
	static char OpenBC() { return openBracketChar; }
	static char CloseBC() { return closeBracketChar; }
	static void SetOpenBC(char c) { openBracketChar = c; }
	static void SetCloseBC(char c) { closeBracketChar = c; }
	// Basic internal data types
public:
	class Token
	{
	public:
		typedef enum
		{
			Empty,
			Constant,
			ConstantInt,
			Variable,
			Operator,
			OpenBracket,
			CloseBracket,
			Function
		} TokenType;
	public:
		Token()
		{
			type = Empty;
			dvalue = 0.0;
			svalue.Empty();
			pvalue = 0;
		}
	public:
		TokenType type;
		double dvalue;
		CString svalue;
		int pvalue;
	};

	class TokenStack : public CTypedPtrList< CPtrList, Token* >
	{
	public:
		~TokenStack(void)
		{
			while(!IsEmpty())
			{
				delete GetHead();
				RemoveHead();
			}
		}
		void operator << ( const Token& t )
		{
			Token* pt = new Token;
			*pt = t;
			AddHead( pt );
		}
		void operator >> ( Token& t )
		{
			if( IsEmpty() )
				t.type = Token::Empty;
			else
			{
				Token* pt = RemoveHead();
				t = *pt;
				delete pt;
			}
		}
	};
	
// Internal functions
protected:
	bool GetToken( Token& token );
	bool PlaceOperator( const Token& t );
	virtual bool PlaceOperand( const Token& t );
	bool GetConstant( Token& value );
	virtual bool GetIdentifier( CString& name );
	bool isdigit( char c );
	bool ApplyOperator( void );
	virtual bool ApplyFunction( void );
	void SetPriority( Token& t );
	void KillUnary( void );
	bool GetFuncResult(const Token &func, const Token &arg, double& res);
	bool Eval(double* res);

// Data
protected:
	EvalError error;
	CString expression;
	TokenStack operators;
	TokenStack operands;
	bool useIntegerArithm;
	bool ConsecutiveCalc;
	static char openBracketChar;
	static char closeBracketChar;
};
