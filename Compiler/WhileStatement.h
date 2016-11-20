#pragma once

#include "Statement.h"
#include "Expression.h"

class WhileStatement : public Statement
{
protected:
	std::unique_ptr<Expression> mExpression;
	std::unique_ptr<Statement> mLoop;

public:
	WhileStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mLoop(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = std::unique_ptr<Expression>(expr); }
	void SetLoop(Statement* stmt) { mLoop = std::unique_ptr<Statement>(stmt); }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
