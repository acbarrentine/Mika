#include "stdafx.h"
#include "IfStatement.h"
#include "Expression.h"

void IfStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mExpression->ResolveType(symbolTable);
	mThenClause->ResolveTypes(symbolTable);
	if (mElseClause)
	{
		mElseClause->ResolveTypes(symbolTable);
	}
}
