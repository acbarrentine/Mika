#include "stdafx.h"
#include "AssignmentExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"

AssignmentExpression::AssignmentExpression(int rootToken)
	: Expression(rootToken)
	, mLeft(nullptr)
	, mRight(nullptr)
	, mOpCode(IllegalInstruction)
{
	Token& t = GCompiler.GetToken(rootToken);
	mTokenType = t.GetType();
}

int AssignmentExpression::GetPrecedence() const
{
	switch (mTokenType)
	{
		case TType::kArrow:
			return 4;

		default:
			GCompiler.Error(mRootToken, "invalid token type for assignment operator");
			return 0;
	}
}

void AssignmentExpression::ResolveType(SymbolTable& symbolTable)
{
	mRight->ResolveType(symbolTable);
	
	Type* leftType = mLeft->GetType();
	Type* rightType = mRight->GetType();
	if (leftType != rightType)
	{
		GCompiler.Error(mRootToken, "incompatible types in expression");
	}
}

void AssignmentExpression::GenCode(ObjectFileHelper& helper)
{
	mRight->GenCode(helper);

	mResultRegister = new IRRegisterOperand();
	IRInstruction* binOp = helper.EmitInstruction(CopyStackToStack, mRootToken);
	binOp->SetOperand(0, new IRVariableOperand(mLeft.get()));
	binOp->SetOperand(1, mRight->GetResultRegister());
}
