#pragma once

#include "Statement.h"

class Expression;
class Label;

class IfStatement : public Statement
{
protected:
	Expression* mExpression;
	Statement* mThenClause;
	Statement* mElseClause;
	Label* mElseLabel;
	Label* mEndLabel;

public:
	IfStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mThenClause(nullptr)
		, mElseClause(nullptr)
		, mElseLabel(nullptr)
		, mEndLabel(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetThenClause(Statement* stmt) { mThenClause = stmt; }
	void SetElseClause(Statement* stmt) { mElseClause = stmt; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
	virtual void CheckReturnStatement(Type* expectedType) override;
};
