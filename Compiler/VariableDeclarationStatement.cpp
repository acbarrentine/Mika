#include "stdafx.h"
#include "VariableDeclarationStatement.h"
#include "ObjectFileHelper.h"
#include "SymbolTable.h"

void VariableDeclarationStatement::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.AddVariable(mVariable.get());
	if (mExpression)
	{
		mExpression->ResolveType(symbolTable);

		// TODO - check compatibility with the variable
	}
}

void VariableDeclarationStatement::GenCode(ObjectFileHelper& helper)
{
	if (mExpression)
	{
		mExpression->GenCode(helper);

		IRInstruction* op = helper.EmitInstruction(CopyStackToStack, mRootToken);
		op->SetOperand(0, new IRVariableOperand(mVariable.get()));
		op->SetOperand(1, mExpression->GetResultRegister());
	}
}
