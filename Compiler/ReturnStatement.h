#pragma once

#include "Statement.h"
#include "Expression.h"

class ReturnStatement : public Statement
{
protected:
	std::unique_ptr<Expression> mExpression;

public:
	ReturnStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = std::unique_ptr<Expression>(expr); }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
