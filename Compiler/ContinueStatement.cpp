#include "stdafx.h"
#include "ContinueStatement.h"
#include "ObjectFileHelper.h"
#include "SymbolTable.h"
#include "Compiler.h"

void ContinueStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mTarget = symbolTable.FindLabel(GCompiler.AddIdentifier("while_loop"));
	if (!mTarget)
	{
		GCompiler.Error(mRootToken, "no loop found for continue statement target");
	}
}

void ContinueStatement::GenCode(ObjectFileHelper& helper)
{
	IRInstruction* branchOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	branchOp->SetOperand(0, new IRLabelOperand(mTarget));
}
