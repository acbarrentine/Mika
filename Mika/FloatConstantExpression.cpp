#include "stdafx.h"
#include "FloatConstantExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"


void FloatConstantExpression::ResolveType(SymbolTable&)
{
	mType = GCompiler.FindType(TType::kFloat);

	Token& tok = GCompiler.GetToken(mRootToken);
	mValue = tok.GetFloatValue();
}

void FloatConstantExpression::GenCode(ObjectFileHelper& helper)
{
	mResultRegister = new IRRegisterOperand;

	IRInstruction& op = helper.EmitInstruction(CopyConstantToStack, mRootToken);
	op.SetOperand(0, mResultRegister);
	op.SetOperand(1, new IRFloatOperand(mValue));
}
