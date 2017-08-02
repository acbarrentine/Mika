#include "stdafx.h"
#include "IntConstantExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"


void IntConstantExpression::ResolveType(SymbolTable&)
{
	mType = GCompiler.GetIntType();
	
	Token& tok = GCompiler.GetToken(mRootToken);
	mValue = tok.GetIntValue();
}

void IntConstantExpression::GenCode(ObjectFileHelper& helper)
{
	mResultRegister = new IRRegisterOperand;

	IRInstruction* op = helper.EmitInstruction(CopyConstantToStack, mRootToken);
	op->SetOperand(0, mResultRegister);
	op->SetOperand(1, new IRIntOperand(mValue));
}
