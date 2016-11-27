#include "stdafx.h"
#include "FunctionCallExpression.h"
#include "Compiler.h"
#include "FunctionDeclaration.h"
#include "ObjectFileHelper.h"
#include "Variable.h"

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

	if (mActuals.size() != mDeclaration->GetParameterCount())
	{
		GCompiler.Error(mRootToken, "wrong number of args for function call");
	}
	else
	{
		for (int i = 0; i < mActuals.size(); ++i)
		{
			Expression* expr = mActuals[i];
			expr->ResolveType(symbolTable);
			
			Variable* declared = mDeclaration->GetParameter(i);
			if (expr->GetType() != declared->GetType())
			{
				GCompiler.Error(expr->GetRootToken(), "argument type incompatible with declaration");
			}
		}

	}
}

void FunctionCallExpression::GenCode(ObjectFileHelper& helper)
{
	for (Expression* expr : mActuals)
	{
		expr->GenCode(helper);
	}

	for (size_t i = 0; i < mActuals.size(); ++i)
	{
		Expression* expr = mActuals[i];
		IRInstruction* pushOp = helper.EmitInstruction(PushArgument, expr->GetRootToken());
		pushOp->SetOperand(0, expr->GetResultRegister());
		pushOp->SetOperand(1, new IRIntOperand(i == 0));
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
