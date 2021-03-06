#pragma once

#include "Expression.h"

class FloatConstantExpression : public Expression
{
protected:
	float mValue;

public:
	FloatConstantExpression(int rootToken)
		: Expression(rootToken)
		, mValue(0.0f)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
