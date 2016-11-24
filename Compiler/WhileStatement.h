#pragma once

#include "Statement.h"
#include "Expression.h"

class WhileStatement : public Statement
{
protected:
	Expression* mExpression;
	Statement* mLoop;

public:
	WhileStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mLoop(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetLoop(Statement* stmt) { mLoop = stmt; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
