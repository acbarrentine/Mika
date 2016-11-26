#include "stdafx.h"
#include "ReturnStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"
#include "Compiler.h"
#include "SymbolTable.h"
#include "IRCode.h"

void ReturnStatement::ResolveTypes(SymbolTable& symbolTable)
{
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
}
