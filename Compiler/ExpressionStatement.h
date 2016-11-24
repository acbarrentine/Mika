#pragma once

#include "Statement.h"

class Expression;

class ExpressionStatement : public Statement
{
protected:
	Expression* mExpression;

public:
	ExpressionStatement(int rootToken)
		: Statement(rootToken)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
