#pragma once
#include "EXPREVAL_API.h"
#include <string>

class EXPREVAL_API SMEExpression
{
public:
	SMEExpression();
	~SMEExpression();
	bool Define(const std::string& exprText);
	double CalcValue();
private:
	void* symbol_table_;
	void* expression_;
	bool error_;
};

