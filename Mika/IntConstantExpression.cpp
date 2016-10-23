#include "stdafx.h"
#include "IntConstantExpression.h"
#include "Compiler.h"

void IntConstantExpression::ResolveType(SymbolTable&)
{
	mType = GCompiler.FindType(TType::kInt);
}
