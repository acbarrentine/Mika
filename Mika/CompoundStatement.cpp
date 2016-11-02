#include "stdafx.h"
#include "CompoundStatement.h"

void CompoundStatement::ResolveTypes(SymbolTable& symbolTable)
{
	for (Statement* stmt : mStmtList)
	{
		stmt->ResolveTypes(symbolTable);
	}
}

void CompoundStatement::GenCode(ObjectFileHelper& helper)
{
	for (Statement* stmt : mStmtList)
	{
		stmt->GenCode(helper);
	}
}
