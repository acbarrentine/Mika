#include "stdafx.h"
#include "StringConstantExpression.h"
#include "Compiler.h"

void StringConstantExpression::ResolveType(SymbolTable&)
{
	mType = GCompiler.FindType(TType::kString);
}
