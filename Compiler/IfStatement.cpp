#include "stdafx.h"
#include "IfStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"
#include "Compiler.h"

void IfStatement::ResolveTypes(SymbolTable& symbolTable)
{
	mExpression->ResolveType(symbolTable);
	mThenClause->ResolveTypes(symbolTable);
	if (mElseClause)
	{
		mElseClause->ResolveTypes(symbolTable);
	}
}

void IfStatement::GenCode(ObjectFileHelper& helper)
{
	IRLabelInstruction* elseLabel = helper.GenLabel(GCompiler.AddIdentifier("if_else"), mRootToken);
	IRLabelInstruction* endLabel = helper.GenLabel(GCompiler.AddIdentifier("if_end"), mRootToken);

	mExpression->GenCode(helper);

	// conditional branch to end
	IRRegisterOperand* condition = mExpression->GetResultRegister();
	IRInstruction* ifOp = helper.EmitInstruction(ConditionalBranch, mRootToken);
	ifOp->SetOperand(0, condition);
	ifOp->SetOperand(1, new IRLabelOperand(elseLabel));

	// then
	mThenClause->GenCode(helper);
	
	// branch to end
	IRInstruction* unOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	unOp->SetOperand(0, new IRLabelOperand(endLabel));

	// else
	helper.EmitLabel(elseLabel);
	if (mElseClause)
	{
		mElseClause->GenCode(helper);
	}

	// end
	helper.EmitLabel(endLabel);
}
