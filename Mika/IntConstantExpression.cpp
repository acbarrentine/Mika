#include "stdafx.h"
#include "IntConstantExpression.h"
#include "Compiler.h"

void IntConstantExpression::ResolveType()
{
	mType = GCompiler.FindType(TType::kInt);
}
