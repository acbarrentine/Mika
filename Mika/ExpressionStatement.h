#pragma once

#include "Statement.h"
class Expression;

class ExpressionStatement : public Statement
{
protected:
	Expression* mExpression;

public:
	ExpressionStatement(size_t rootToken)
		: Statement(rootToken)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
};
