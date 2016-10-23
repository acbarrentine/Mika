#include "stdafx.h"
#include "FloatConstantExpression.h"
#include "Compiler.h"

void FloatConstantExpression::ResolveType(SymbolTable&)
{
	mType = GCompiler.FindType(TType::kFloat);
}
