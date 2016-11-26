#pragma once

#include "Statement.h"

class Expression;
class Label;

class WhileStatement : public Statement
{
protected:
	Expression* mExpression;
	Statement* mLoop;
	Label* mLoopLabel;
	Label* mEndLabel;

public:
	WhileStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mLoop(nullptr)
		, mLoopLabel(nullptr)
		, mEndLabel(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetLoop(Statement* stmt) { mLoop = stmt; }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
