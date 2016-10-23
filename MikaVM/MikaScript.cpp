#include "stdafx.h"
#include "MikaScript.h"

MikaCell* MikaScript::OpCode::GetArgs() const
{
	return mNumArgs ? (MikaCell*)(this + 1) : nullptr;
}

MikaScript::OpCode* MikaScript::OpCode::Next() const
{
	const char* thisOp = (const char*) this;
	int argBytes = mNumArgs * sizeof(MikaCell);
	const char* nextOp = thisOp + argBytes;
	return (MikaScript::OpCode*) nextOp;
}
