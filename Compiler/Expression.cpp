#include "stdafx.h"
#include "Expression.h"
#include "Compiler.h"

void Expression::GenAssign(ObjectFileHelper&, IROperand*)
{
	GCompiler.Error(mRootToken, "invalid target for assignment");
}
