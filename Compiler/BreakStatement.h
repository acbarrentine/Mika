#pragma once

#include "Statement.h"

class Label;

class BreakStatement : public Statement
{
protected:
	Label* mTarget;

public:
	BreakStatement(int rootToken)
		: Statement(rootToken)
		, mTarget(nullptr)
	{
	}

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
