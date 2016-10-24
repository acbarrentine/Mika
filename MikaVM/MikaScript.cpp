#include "stdafx.h"
#include "MikaScript.h"
#include "MikaArchive.h"


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

void MikaScript::ReadScript(const char* fileName)
{
	MikaReader reader;
	reader.Open(fileName);
	if (!reader.Failed())
	{
		MikaFileHeader fileHeader;
		reader >> fileHeader;
	}
}

