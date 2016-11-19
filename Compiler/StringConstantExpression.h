#pragma once

#include "Expression.h"
#include "Identifier.h"

class StringConstantExpression : public Expression
{
protected:
	Identifier mValue;

public:
	StringConstantExpression(int rootToken)
		: Expression(rootToken)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
