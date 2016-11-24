#pragma once

#include "Statement.h"
#include "Expression.h"

class ReturnStatement : public Statement
{
protected:
	Expression* mExpression;

public:
	ReturnStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
