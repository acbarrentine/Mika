#include "stdafx.h"
#include "FunctionCallExpression.h"
#include "Compiler.h"
#include "FunctionDeclaration.h"
#include "ObjectFileHelper.h"


void FunctionCallExpression::ResolveType(SymbolTable& symbolTable)
{
	Token& nameToken = GCompiler.GetToken(mRootToken);
	mDeclaration = GCompiler.FindDeclaration(nameToken.GetIdentifier());

	if (!mDeclaration)
	{
		GCompiler.Error(mRootToken, "function declaration not found");
		return;
	}

	mType = mDeclaration->GetReturnType();

	for (size_t i = 0; i < mActuals.size(); ++i)
	{
		Expression* expr = mActuals[i];
		expr->ResolveType(symbolTable);

		// todo - compare to declared types
	}
}

void FunctionCallExpression::GenCode(ObjectFileHelper& helper)
{
	for (size_t i = 0; i < mActuals.size(); ++i)
	{
		Expression* expr = mActuals[i];
		expr->GenCode(helper);
	}

	for (size_t i = 0; i < mActuals.size(); ++i)
	{
		Expression* expr = mActuals[i];
		IRInstruction& op = helper.EmitInstruction(PushArgument, expr->GetRootToken());
		op.SetOperand(0, expr->GetResultRegister());
	}

	IRInstruction& op = helper.EmitInstruction(CallFunction, mRootToken);
	op.SetOperand(0, new IRFunctionOperand(mDeclaration));

	if (mType != GCompiler.FindType(TType::kVoid))
	{
		mResultRegister = new IRRegisterOperand;

		op = helper.EmitInstruction(CopyResultRegister, mRootToken);
		op.SetOperand(0, mResultRegister);
	}
}
