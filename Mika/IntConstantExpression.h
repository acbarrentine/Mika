#pragma once

#include "Expression.h"

class IntConstantExpression : public Expression
{
protected:
	int mValue;

public:
	IntConstantExpression(int rootToken)
		: Expression(rootToken)
		, mValue(0)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
