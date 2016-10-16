#pragma once

#include "Expression.h"

class IntConstantExpression : public Expression
{
protected:
	int mValue;

public:
	IntConstantExpression(size_t rootToken)
		: Expression(rootToken)
		, mValue(0)
	{}

	virtual void ResolveType();
};
