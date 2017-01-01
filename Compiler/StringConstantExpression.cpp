#include "stdafx.h"
#include "StringConstantExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"


void StringConstantExpression::ResolveType(SymbolTable&)
{
	mType = GCompiler.FindType(TType::kString);

	Token& tok = GCompiler.GetToken(mRootToken);
	mValue = tok.GetStringValue();
}

void StringConstantExpression::GenCode(ObjectFileHelper& helper)
{
	mResultRegister = new IRRegisterOperand;
	IRInstruction* op = helper.EmitInstruction(CopyConstantToStack, mRootToken);
	op->SetOperand(0, mResultRegister);
	op->SetOperand(1, new IRStringOperand(mValue));
}
