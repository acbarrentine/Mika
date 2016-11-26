#pragma once

#include "Statement.h"

class Label;

class ContinueStatement : public Statement
{
protected:
	Label* mTarget;

public:
	ContinueStatement(int rootToken)
		: Statement(rootToken)
		, mTarget(nullptr)
	{
	}

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
