#pragma once

class Variable;
class ScriptFunction;

class ObjectFileHelper
{
protected:
	struct FunctionRecord
	{

	};

	const char* mFileName;

public:
	ObjectFileHelper(const char* outputFileName)
		: mFileName(outputFileName)
	{
	}

	void AddFunction(ScriptFunction* func);
	void AddVariable(Variable* var);
};
