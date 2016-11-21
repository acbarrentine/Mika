#include "stdafx.h"
#include "AssignmentExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"

AssignmentExpression::AssignmentExpression(int rootToken)
	: BinaryExpression(rootToken)
{
}

void AssignmentExpression::ResolveType(SymbolTable& symbolTable)
{
	mLeft->ResolveType(symbolTable);
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
	mLeft->GenAssign(helper, mRight->GetResultRegister());
}
