#pragma once

#include "Expression.h"
#include "Identifier.h"

class IdentifierExpression : public Expression
{
protected:
	Identifier mValue;

public:
	IdentifierExpression(size_t rootToken)
		: Expression(rootToken)
	{}

	virtual void ResolveType();
};
