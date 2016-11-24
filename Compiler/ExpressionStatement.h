#pragma once

#include "Statement.h"
#include "Expression.h"

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
