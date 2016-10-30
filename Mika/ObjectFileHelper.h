#pragma once

#include "Identifier.h"
class Variable;
class ScriptFunction;

class ObjectFileHelper
{
protected:
	struct FunctionRecord
	{
		ScriptFunction* mFunction;
		int mNameOffset;
	};

	const char* mFileName;
	std::vector<char> mStringData;
	std::vector<FunctionRecord> mFunctions;

public:
	ObjectFileHelper(const char* outputFileName)
		: mFileName(outputFileName)
	{
	}

	void AddFunction(ScriptFunction* func);
	void AddVariable(Variable* var);

	void WriteFile();

protected:
	int AddString(Identifier id);
};
