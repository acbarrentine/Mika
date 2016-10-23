#pragma once

#include "Statement.h"
class Expression;

class IfStatement : public Statement
{
protected:
	Expression* mExpression;
	Statement* mThenClause;
	Statement* mElseClause;

public:
	IfStatement(size_t rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mThenClause(nullptr)
		, mElseClause(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetThenClause(Statement* stmt) { mThenClause = stmt; }
	void SetElseClause(Statement* stmt) { mElseClause = stmt; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
};
