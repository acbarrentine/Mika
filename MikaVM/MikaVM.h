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
	MikaCell mConditionRegister;

public:
	MikaVM()
	{}

	void Execute(const char* functionName);
};
