#pragma once

#include "Expression.h"
#include "Identifier.h"

class StringConstantExpression : public Expression
{
protected:
	Identifier mValue;

public:
	StringConstantExpression(size_t rootToken)
		: Expression(rootToken)
	{}

	virtual void ResolveType(SymbolTable& symbolTable);
};
