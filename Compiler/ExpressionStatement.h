#pragma once

#include "Statement.h"
#include "Expression.h"

class ExpressionStatement : public Statement
{
protected:
	std::unique_ptr<Expression> mExpression;

public:
	ExpressionStatement(int rootToken)
		: Statement(rootToken)
	{}

	void SetExpression(Expression* expr) { mExpression = std::unique_ptr<Expression>(expr); }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
