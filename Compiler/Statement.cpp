#include "stdafx.h"
#include "Statement.h"
#include "Compiler.h"


void Statement::CheckReturnStatement(Type* expectedType)
{
    if (expectedType != GCompiler.GetVoidType())
    {
        GCompiler.Error(mRootToken, "return statement expected");
    }
}
