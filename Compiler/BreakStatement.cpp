#include "stdafx.h"
#include "BreakStatement.h"
#include "ObjectFileHelper.h"
#include "SymbolTable.h"
#include "Compiler.h"

void BreakStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mTarget = symbolTable.FindLabel(GCompiler.AddIdentifier("while_end"));
	if (!mTarget)
	{
		GCompiler.Error(mRootToken, "no loop found for break statement target");
	}
}

void BreakStatement::GenCode(ObjectFileHelper& helper)
{
	IRInstruction* branchOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	branchOp->SetOperand(0, new IRLabelOperand(mTarget));
}
