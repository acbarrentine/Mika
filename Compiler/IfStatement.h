#pragma once

#include "Statement.h"
#include "Expression.h"

class IfStatement : public Statement
{
protected:
	std::unique_ptr<Expression> mExpression;
	std::unique_ptr<Statement> mThenClause;
	std::unique_ptr<Statement> mElseClause;

public:
	IfStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mThenClause(nullptr)
		, mElseClause(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = std::unique_ptr<Expression>(expr); }
	void SetThenClause(Statement* stmt) { mThenClause = std::unique_ptr<Statement>(stmt); }
	void SetElseClause(Statement* stmt) { mElseClause = std::unique_ptr<Statement>(stmt); }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
