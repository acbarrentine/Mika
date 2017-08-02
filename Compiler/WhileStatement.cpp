#include "stdafx.h"
#include "WhileStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"
#include "Compiler.h"
#include "SymbolTable.h"

void WhileStatement::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.Push();

	int seq = symbolTable.GetNextLabelSequence();
	mLoopLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("while_loop"), seq);
	mEndLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("while_end"), seq);

	mExpression->ResolveType(symbolTable);
	if (mExpression->GetType() != GCompiler.GetIntType())
	{
		GCompiler.Error(mExpression->GetRootToken(), "incompatible type for while condition");
	}

	mLoop->ResolveTypes(symbolTable);

	symbolTable.Pop();
}

void WhileStatement::GenCode(ObjectFileHelper& helper)
{
	// evaluate the loop condition
	helper.EmitLabel(mLoopLabel, mRootToken);
	mExpression->GenCode(helper);

	// conditional branch to end
	IROperand* condition = mExpression->GetResultRegister();
	IRInstruction* endBranchOp = helper.EmitInstruction(ConditionalBranch, mRootToken);
	endBranchOp->SetOperand(0, condition);
	endBranchOp->SetOperand(1, new IRLabelOperand(mEndLabel));
	endBranchOp->SetOperand(2, new IRStackBytesOperand(false));

	// body
	mLoop->GenCode(helper);

	// branch to start
	IRInstruction* loopBranchOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	loopBranchOp->SetOperand(0, new IRLabelOperand(mLoopLabel));
	loopBranchOp->SetOperand(1, new IRStackBytesOperand(false));

	// end
	helper.EmitLabel(mEndLabel, mRootToken);
}
