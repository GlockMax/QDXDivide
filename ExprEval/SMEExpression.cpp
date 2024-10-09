#include "pch.h"
#include "SMEExpression.h"
#undef min
#undef max
#include "ExprTk.hpp"

SMEExpression::SMEExpression()
{
	symbol_table_ = new exprtk::symbol_table<double>;
	expression_ = new exprtk::expression<double>;
	error_ = false;
}

SMEExpression::~SMEExpression()
{
	delete symbol_table_;
	delete expression_;
}

bool SMEExpression::Define(const std::string& exprText)
{
	exprtk::symbol_table<double>& symbol_table = *static_cast<exprtk::symbol_table<double>*>(symbol_table_);
	exprtk::expression<double>& expression = *static_cast<exprtk::expression<double>*>(expression_);
	symbol_table.clear();
	expression.register_symbol_table(symbol_table);
	exprtk::parser<double> parser;
	error_ = !parser.compile(exprText, expression);
	return !error_;
}

double SMEExpression::CalcValue()
{
	if (error_)
		return 0.;
	exprtk::expression<double>& expression = *static_cast<exprtk::expression<double>*>(expression_);
	return expression.value();
}
