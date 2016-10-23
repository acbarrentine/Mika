#pragma once

#include "Statement.h"
class Expression;

class WhileStatement : public Statement
{
protected:
	Expression* mExpression;
	Statement* mLoop;

public:
	WhileStatement(size_t rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mLoop(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetLoop(Statement* stmt) { mLoop = stmt; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
};
