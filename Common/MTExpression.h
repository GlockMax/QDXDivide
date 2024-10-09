#pragma once
#include "EXPREVAL_API.h"
#include <string>
#include <vector>

class EXPREVAL_API MTExpression
{
public:
	MTExpression();
	~MTExpression();
	bool Define(const std::string& exprText, const std::vector<std::string>& varNames, const std::vector<int>& varIndexes);
	const std::vector<int>& GetIndexes() const { return axIndexes_; }
	double CalcValue(const std::vector<double>& values);
private:
	void* symbol_table_;
	void* expression_;
	std::vector<int> axIndexes_;
	std::vector<double> varValues_;
	bool error_;
};

