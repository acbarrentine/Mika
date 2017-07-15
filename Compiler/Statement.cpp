#include "stdafx.h"
#include "Statement.h"
#include "Compiler.h"


void Statement::CheckReturnStatement(Type* expectedType)
{
    if (expectedType != GCompiler.FindType(TType::kVoid))
    {
        GCompiler.Error(mRootToken, "return statement expected");
    }
}
