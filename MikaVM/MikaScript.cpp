#include "stdafx.h"
#include "MikaScript.h"
#include "MikaArchive.h"


MikaScript::Cell* MikaScript::OpCode::GetArgs() const
{
	return mNumArgs ? (Cell*)(this + 1) : nullptr;
}

MikaScript::OpCode* MikaScript::OpCode::Next() const
{
	const char* thisOp = (const char*) this;
	int argBytes = mNumArgs * sizeof(Cell);
	const char* nextOp = thisOp + argBytes;
	return (MikaScript::OpCode*) nextOp;
}

MikaScript::MikaScript(const char* fileName)
{
	MikaReader reader;
	reader.Process(fileName, this);
}

