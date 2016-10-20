#include "stdafx.h"
#include "ExpressionStatement.h"
#include "Expression.h"

void ExpressionStatement::ResolveTypes()
{
	mExpression->ResolveType();
}
