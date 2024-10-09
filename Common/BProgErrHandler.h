#pragma once
#include "UNITS_API.h"
#include "NCUnitStatePrev.h"

class UNITS_API BProgErrHandler
{
public:
	enum ErrorReaction
	{
		ER_BREAK,
		ER_SKIP_LINE,
		ER_CONTINUE,
		ER_IGNORE
	};
	enum ErrorLevel
	{
		EL_CRITICAL_ERROR = 0,
		EL_LINE_ERROR = 1,
		EL_LEXEM_ERROR = 2,
		EL_CHECK_ERROR = 3,
	};
	BProgErrHandler(int ReactType) noexcept;
	~BProgErrHandler() noexcept;
	ErrorReaction GetErrorReaction(NCUnitStatePrev::EvaluateCadrResult ret, int ErrCode) const noexcept;
	ErrorLevel GetErrorLevel(int ErrCode) const noexcept;
	CString MakeErrorMessage(const class NProgError& ProgError) const;
	int SetReactType(int ReactType);// returns old ReactType
protected:
	void SetErrorLevel(int ErrCode, ErrorLevel ErrLevel) noexcept;
protected:
	static constexpr size_t MAX_ERRORS = 250;
	static constexpr size_t MAX_ERROR_LEVELS = 4;
	static constexpr size_t ERROR_NUM_BASE = 1000;
	ErrorLevel ErrLevels[MAX_ERRORS];
	ErrorReaction ErrReactions[MAX_ERROR_LEVELS];
	int CurReactType;
};

