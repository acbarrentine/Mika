#include "stdafx.h"
#include "ExpressionStatement.h"
#include "Expression.h"

void ExpressionStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mExpression->ResolveType(symbolTable);
}

void ExpressionStatement::GenCode(ObjectFileHelper& helper)
{
	//mExpression->GetCode(helper);
	helper;
}
