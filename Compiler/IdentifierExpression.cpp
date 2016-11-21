#include "stdafx.h"
#include "IdentifierExpression.h"
#include "Variable.h"
#include "SymbolTable.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"


void IdentifierExpression::ResolveType(SymbolTable& symbolTable)
{
	Token& tok = GCompiler.GetToken(mRootToken);
	mValue = tok.GetStringValue();
	mVariable = symbolTable.FindVariable(mValue);
	if (!mVariable)
	{
		GCompiler.Error(mRootToken, "identifier not found");
		mType = GCompiler.FindType(TType::kInt);
	}
	else
	{
		mType = mVariable->GetType();
	}
}

void IdentifierExpression::GenCode(ObjectFileHelper& helper)
{
	mResultRegister = new IRRegisterOperand;

	IRInstruction* op = helper.EmitInstruction(CopyStackToStack, mRootToken);
	op->SetOperand(0, mResultRegister);
	op->SetOperand(1, new IRVariableOperand(mVariable));
}

void IdentifierExpression::GenAssign(ObjectFileHelper& helper, IRRegisterOperand* src)
{
	if (mVariable)
	{
		IRInstruction* op = helper.EmitInstruction(CopyStackToStack, mRootToken);
		op->SetOperand(0, new IRVariableOperand(mVariable));
		op->SetOperand(1, src);
	}
	else
	{
		GCompiler.Error(mRootToken, "invalid target for assignment");
	}
}