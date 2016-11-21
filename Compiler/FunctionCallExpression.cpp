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

	for (unsigned int i = 0; i < mActuals.size(); ++i)
	{
		Expression* expr = mActuals[i];
		expr->ResolveType(symbolTable);

		// todo - compare to declared types
	}
}

void FunctionCallExpression::GenCode(ObjectFileHelper& helper)
{
	for (Expression* expr : mActuals)
	{
		expr->GenCode(helper);
	}

	for (Expression* expr : mActuals)
	{
		IRInstruction* pushOp = helper.EmitInstruction(PushArgument, expr->GetRootToken());
		pushOp->SetOperand(0, expr->GetResultRegister());
	}

	OpCode code = mDeclaration->IsScriptFunction() ? CallScriptFunction : CallNativeFunction;
	IRInstruction* callOp = helper.EmitInstruction(code, mRootToken);
	callOp->SetOperand(0, new IRFunctionOperand(mDeclaration));

	if (mType != GCompiler.FindType(TType::kVoid))
	{
		mResultRegister = new IRRegisterOperand;

		IRInstruction* copyOp = helper.EmitInstruction(CopyResultRegister, mRootToken);
		copyOp->SetOperand(0, mResultRegister);
	}
}
