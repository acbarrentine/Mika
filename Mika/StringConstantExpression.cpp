#include "stdafx.h"
#include "StringConstantExpression.h"
#include "Compiler.h"

void StringConstantExpression::ResolveType()
{
	mType = GCompiler.FindType(TType::kString);
}
