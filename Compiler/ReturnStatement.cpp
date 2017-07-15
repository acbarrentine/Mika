#include "stdafx.h"
#include "ReturnStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"
#include "Compiler.h"
#include "SymbolTable.h"

void ReturnStatement::ResolveTypes(SymbolTable& symbolTable)
{
	if (mExpression)
		mExpression->ResolveType(symbolTable);
	mTarget = symbolTable.FindLabel(GCompiler.AddIdentifier("end"));
}

void ReturnStatement::GenCode(ObjectFileHelper& helper)
{
	if (mExpression)
	{
		mExpression->GenCode(helper);

		IRInstruction* op = helper.EmitInstruction(SetResultRegister, mRootToken);
		op->SetOperand(0, mExpression->GetResultRegister());
	}

	IRInstruction* branchOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	branchOp->SetOperand(0, new IRLabelOperand(mTarget));
	branchOp->SetOperand(1, new IRStackBytesOperand(false));
}

void ReturnStatement::CheckReturnStatement(Type* expectedType)
{
	Type* voidType = GCompiler.FindType(TType::kVoid);
	if (expectedType == voidType)
	{
		if (mExpression)
		{
			GCompiler.Error(mRootToken, "unexpected return type");
		}
	}
	else
	{
		if (!mExpression || mExpression->GetType() != expectedType)
		{
			GCompiler.Error(mRootToken, "unexpected type for return statement");
		}		
	}
}
