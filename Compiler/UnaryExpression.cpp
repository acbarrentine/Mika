#include "stdafx.h"
#include "UnaryExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"

UnaryExpression::UnaryExpression(int rootToken)
	: Expression(rootToken)
	, mTarget(nullptr)
	, mOpCode(IllegalInstruction)
{}

void UnaryExpression::ResolveType(SymbolTable& symbolTable)
{
	mTarget->ResolveType(symbolTable);
	mType = mTarget->GetType();

	Token& tok = GCompiler.GetToken(mRootToken);
	if (tok.GetType() != TType::kMinus)
	{
		GCompiler.Error(mRootToken, "illegal token for negation");
	}

	Type* intType = GCompiler.GetIntType();
	Type* floatType = GCompiler.GetFloatType();
	if (mType == intType)
	{
		mOpCode = NegateInt;
	}
	else if (mType == floatType)
	{
		mOpCode = NegateFloat;
	}
	else
	{
		GCompiler.Error(mTarget->GetRootToken(), "illegal type for unary expression");
	}
}

void UnaryExpression::GenCode(ObjectFileHelper& helper)
{
	mTarget->GenCode(helper);

	mResultRegister = new IRRegisterOperand();
	IRInstruction* op = helper.EmitInstruction(mOpCode, mRootToken);
	op->SetOperand(0, mResultRegister);
	op->SetOperand(1, mTarget->GetResultRegister());
}
