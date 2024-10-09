#include "pch.h"
#include "MTExpression.h"
#undef min
#undef max
#include "ExprTk.hpp"

MTExpression::MTExpression()
{
	symbol_table_ = new exprtk::symbol_table<double>;
	expression_ = new exprtk::expression<double>;
	error_ = false;
}

MTExpression::~MTExpression()
{
	delete symbol_table_;
	delete expression_;
}

bool MTExpression::Define(const std::string& exprText, const std::vector<std::string>& varNames, const std::vector<int>& varIndexes)
{
	if (varNames.size() != varIndexes.size())
		return false;
	exprtk::symbol_table<double>& symbol_table = *static_cast<exprtk::symbol_table<double>*>(symbol_table_);
	exprtk::expression<double>& expression = *static_cast<exprtk::expression<double>*>(expression_);
	axIndexes_ = varIndexes;
	varValues_.resize(varNames.size());
	symbol_table.clear();
	for (int i = 0; i < varNames.size(); ++i)
		symbol_table.add_variable(varNames[i], varValues_[i]);
	expression.register_symbol_table(symbol_table);
	exprtk::parser<double> parser;
	error_ = !parser.compile(exprText, expression);
	return !error_;
}

double MTExpression::CalcValue(const std::vector<double>& values)
{
	if (error_)
		return 0.;
	if(values.size() != varValues_.size())
		return 0.;
	for (int i = 0; i < values.size(); ++i)
		varValues_[i] = values[i];
	exprtk::expression<double>& expression = *static_cast<exprtk::expression<double>*>(expression_);
	return expression.value();
}
