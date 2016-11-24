#include "stdafx.h"
#include "VariableDeclarationStatement.h"
#include "ObjectFileHelper.h"
#include "SymbolTable.h"
#include "Compiler.h"
#include "Expression.h"
#include "Variable.h"

void VariableDeclarationStatement::SetGlobal(bool isGlobal)
{
	mVariable->SetIsGlobal(isGlobal);
}

void VariableDeclarationStatement::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.AddVariable(mVariable);
	if (mExpression)
	{
		mExpression->ResolveType(symbolTable);

		if (mExpression->GetType() != mVariable->GetType())
		{
			GCompiler.Error(mExpression->GetRootToken(), "initializer type incompatible with variable type");
		}
	}
}

void VariableDeclarationStatement::GenCode(ObjectFileHelper& helper)
{
	if (mExpression)
	{
		mExpression->GenCode(helper);

		IRInstruction* op = helper.EmitInstruction(CopyStackToStack, mRootToken);
		op->SetOperand(0, new IRVariableOperand(mVariable));
		op->SetOperand(1, mExpression->GetResultRegister());
	}
}
