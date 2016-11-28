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
	IRInstruction* pushScope = helper.EmitInstruction(MoveStackPointer, mRootToken);
	pushScope->SetOperand(0, new IRStackBytesOperand(false));

	for (auto& stmt : mStmtList)
	{
		stmt->GenCode(helper);
	}

	IRInstruction* popScope = helper.EmitInstruction(MoveStackPointer, mRootToken);
	popScope->SetOperand(0, new IRStackBytesOperand(true));
}
