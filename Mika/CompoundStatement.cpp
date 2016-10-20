#include "stdafx.h"
#include "CompoundStatement.h"

void CompoundStatement::ResolveTypes()
{
	for (size_t i = 0; i < mStmtList.size(); ++i)
	{
		Statement* stmt = mStmtList[i];
		stmt->ResolveTypes();
	}
}
