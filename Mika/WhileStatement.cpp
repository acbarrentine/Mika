#include "stdafx.h"
#include "WhileStatement.h"
#include "Expression.h"

void WhileStatement::ResolveTypes()
{
	mExpression->ResolveType();
	mLoop->ResolveTypes();
}
