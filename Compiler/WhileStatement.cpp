#include "stdafx.h"
#include "WhileStatement.h"
#include "Expression.h"

void WhileStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mExpression->ResolveType(symbolTable);
	mLoop->ResolveTypes(symbolTable);
}

void WhileStatement::GenCode(ObjectFileHelper& helper)
{
	helper;
}
