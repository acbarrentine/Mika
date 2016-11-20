#include "stdafx.h"
#include "CompoundStatement.h"

void CompoundStatement::ResolveTypes(SymbolTable& symbolTable)
{
	for (auto& stmt : mStmtList)
	{
		stmt->ResolveTypes(symbolTable);
	}
}

void CompoundStatement::GenCode(ObjectFileHelper& helper)
{
	for (auto& stmt : mStmtList)
	{
		stmt->GenCode(helper);
	}
}
