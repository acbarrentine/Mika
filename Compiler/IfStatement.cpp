#include "stdafx.h"
#include "IfStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"
#include "Compiler.h"
#include "SymbolTable.h"

void IfStatement::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.Push();

	int seq = symbolTable.GetNextLabelSequence();
	mElseLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("if_else"), seq);
	mEndLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("if_end"), seq);

	mExpression->ResolveType(symbolTable);
	mThenClause->ResolveTypes(symbolTable);
	if (mElseClause)
	{
		mElseClause->ResolveTypes(symbolTable);
	}

	symbolTable.Pop();
}

void IfStatement::GenCode(ObjectFileHelper& helper)
{
	mExpression->GenCode(helper);

	// conditional branch to end
	IROperand* condition = mExpression->GetResultRegister();
	IRInstruction* ifOp = helper.EmitInstruction(ConditionalBranch, mRootToken);
	ifOp->SetOperand(0, condition);
	ifOp->SetOperand(1, mElseClause ? new IRLabelOperand(mElseLabel) : new IRLabelOperand(mEndLabel));

	// then
	mThenClause->GenCode(helper);
	
	// branch to end
	IRInstruction* unOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	unOp->SetOperand(0, new IRLabelOperand(mEndLabel));

	// else
	if (mElseClause)
	{
		helper.EmitLabel(mElseLabel, mRootToken);
		mElseClause->GenCode(helper);
	}

	// end
	helper.EmitLabel(mEndLabel, mRootToken);
}
