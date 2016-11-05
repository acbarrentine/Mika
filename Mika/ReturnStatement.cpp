#include "stdafx.h"
#include "ReturnStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"


void ReturnStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mExpression->ResolveType(symbolTable);
}

void ReturnStatement::GenCode(ObjectFileHelper& helper)
{
	if (mExpression)
	{
		mExpression->GenCode(helper);

		IRInstruction& op = helper.EmitInstruction(SetResultRegister, mRootToken);
		op.SetOperand(0, mExpression->GetResultRegister());
	}

	helper.EmitInstruction(IllegalInstruction, mRootToken);
}
