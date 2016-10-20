#include "stdafx.h"
#include "FunctionCallExpression.h"
#include "Compiler.h"
#include "FunctionDeclaration.h"

void FunctionCallExpression::ResolveType()
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
		expr->ResolveType();

		// todo - compare to declared types
	}
}
