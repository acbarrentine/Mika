#pragma once

#include "Statement.h"

class CompoundStatement : public Statement
{
protected:
	std::vector<std::unique_ptr<Statement>> mStmtList;

public:
	CompoundStatement(int rootToken)
		: Statement(rootToken)
	{}

	void AddStatement(Statement* stmt)
	{
		mStmtList.push_back(std::unique_ptr<Statement>(stmt));
	}

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
