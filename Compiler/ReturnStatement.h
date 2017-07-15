#pragma once

#include "Statement.h"

class Expression;
class Label;

class ReturnStatement : public Statement
{
protected:
	Expression* mExpression;
	Label* mTarget;

public:
	ReturnStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mTarget(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
	virtual void CheckReturnStatement(Type* expectedType) override;
};
