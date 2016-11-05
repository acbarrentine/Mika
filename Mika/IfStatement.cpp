#include "stdafx.h"
#include "IfStatement.h"
#include "Expression.h"
#include "ObjectFileHelper.h"


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
	IRLabelOperand* elseLabel = new IRLabelOperand();
	IRLabelOperand* endLabel = new IRLabelOperand();

	mExpression->GenCode(helper);

	// conditional branch to end
	IRRegisterOperand* condition = mExpression->GetResultRegister();
	IRInstruction& ifOp = helper.EmitInstruction(ConditionalBranch, mRootToken);
	ifOp.SetOperand(0, condition);
	ifOp.SetOperand(1, elseLabel);

	// then
	mThenClause->GenCode(helper);
	
	// branch to end
	IRInstruction& unOp = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	unOp.SetOperand(0, endLabel);

	// else
	elseLabel->SetOffset(helper.GetByteCodeOffset());
	if (mElseClause)
	{
		mElseClause->GenCode(helper);
	}

	// end
	endLabel->SetOffset(helper.GetByteCodeOffset());
}
