#pragma once

#include "Statement.h"
#include "Expression.h"

class IfStatement : public Statement
{
protected:
	Expression* mExpression;
	Statement* mThenClause;
	Statement* mElseClause;

public:
	IfStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mThenClause(nullptr)
		, mElseClause(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetThenClause(Statement* stmt) { mThenClause = stmt; }
	void SetElseClause(Statement* stmt) { mElseClause = stmt; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
