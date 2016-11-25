#include "stdafx.h"
#include "WhileStatement.h"
#include "Expression.h"
#include "IRCode.h"
#include "ObjectFileHelper.h"
#include "Compiler.h"

void WhileStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mExpression->ResolveType(symbolTable);
	mLoop->ResolveTypes(symbolTable);
}

void WhileStatement::GenCode(ObjectFileHelper& helper)
{
	IRLabelInstruction* loopLabel = helper.GenLabel(GCompiler.AddIdentifier("while_loop"), mRootToken);
	IRLabelInstruction* endLabel = helper.GenLabel(GCompiler.AddIdentifier("while_end"), mRootToken);

	// evaluate the loop condition
	helper.EmitLabel(loopLabel);
	mExpression->GenCode(helper);

	// conditional branch to end
	IROperand* condition = mExpression->GetResultRegister();
	IRInstruction* endBranchOp = helper.EmitInstruction(ConditionalBranch, mRootToken);
	endBranchOp->SetOperand(0, condition);
	endBranchOp->SetOperand(1, new IRLabelOperand(endLabel));

	// body
	mLoop->GenCode(helper);

	// branch to start
	IRInstruction* loopBranchOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	loopBranchOp->SetOperand(0, new IRLabelOperand(loopLabel));

	// end
	helper.EmitLabel(endLabel);
}
