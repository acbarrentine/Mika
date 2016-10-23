#include "stdafx.h"
#include "IdentifierExpression.h"
#include "Variable.h"
#include "SymbolTable.h"
#include "Compiler.h"


void IdentifierExpression::ResolveType(SymbolTable& symbolTable)
{
	Variable* var = symbolTable.FindVariable(mValue);
	if (!var)
	{
		GCompiler.Error(mRootToken, "identifier not found");
		mType = GCompiler.FindType(TType::kInt);
	}
	else
	{
		mType = var->GetType();
	}
}
