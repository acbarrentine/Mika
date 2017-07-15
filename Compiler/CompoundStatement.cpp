#include "stdafx.h"
#include "CompoundStatement.h"
#include "SymbolTable.h"
#include "ObjectFileHelper.h"

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
	helper.PushScope(mRootToken);

	for (auto& stmt : mStmtList)
	{
		stmt->GenCode(helper);
	}

	helper.PopScope(mRootToken);
}

void CompoundStatement::CheckReturnStatement(Type* expectedType)
{
	if (mStmtList.empty())
	{
		Statement::CheckReturnStatement(expectedType);
	}
	else
	{
		mStmtList.back()->CheckReturnStatement(expectedType);
	}
}
