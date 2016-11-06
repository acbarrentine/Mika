#pragma once

#include "MikaScript.h"

class MikaVM
{
protected:
	struct Location
	{
		const char* Function;
		const char* SourceFile;
		unsigned int LineNumber;
	} mLoc;
	MikaScript::Cell mConditionRegister;

public:
	MikaVM()
	{}

	void Import(MikaScript& script);
	void Execute(const char* functionName);
};
