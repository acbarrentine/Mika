#include "stdafx.h"
#include "IfStatement.h"
#include "Expression.h"

void IfStatement::ResolveTypes()
{
	mExpression->ResolveType();
	mThenClause->ResolveTypes();
	if (mElseClause)
	{
		mElseClause->ResolveTypes();
	}
}
