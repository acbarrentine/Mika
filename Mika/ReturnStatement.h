#pragma once

#include "Statement.h"
class Expression;

class ReturnStatement : public Statement
{
protected:
	Expression* mExpression;

public:
	ReturnStatement(size_t rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
};
