#include "stdafx.h"
#include "CompoundStatement.h"
#include "SymbolTable.h"

void CompoundStatement::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.Push();
	for (auto& stmt : mStmtList)
	{
		stmt->ResolveTypes(symbolTable);
	}
	symbolTable.Pop();
}

void CompoundStatement::GenCode(ObjectFileHelper& helper)
{
	for (auto& stmt : mStmtList)
	{
		stmt->GenCode(helper);
	}
}
