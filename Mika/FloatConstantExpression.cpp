#include "stdafx.h"
#include "FloatConstantExpression.h"
#include "Compiler.h"

void FloatConstantExpression::ResolveType()
{
	mType = GCompiler.FindType(TType::kFloat);
}
