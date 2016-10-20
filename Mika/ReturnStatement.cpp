#include "stdafx.h"
#include "ReturnStatement.h"
#include "Expression.h"

void ReturnStatement::ResolveTypes()
{
	mExpression->ResolveType();
}
