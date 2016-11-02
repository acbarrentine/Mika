#pragma once

#include "Statement.h"

class CompoundStatement : public Statement
{
protected:
	std::vector<Statement*> mStmtList;

public:
	CompoundStatement(size_t rootToken)
		: Statement(rootToken)
	{}

	void AddStatement(Statement* stmt)
	{
		mStmtList.push_back(stmt);
	}

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
