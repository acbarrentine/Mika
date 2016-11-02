#pragma once

#include "Identifier.h"
#include "IRCode.h"

class Variable;
class ScriptFunction;

class ObjectFileHelper
{
protected:
	struct FunctionRecord
	{
		ScriptFunction* mFunction;
		int mNameOffset;
		int mByteCodeOffset;
		std::vector<IRInstruction> mInstructions;

		FunctionRecord(ScriptFunction* func, int nameOffset)
			: mFunction(func)
			, mNameOffset(nameOffset)
			, mByteCodeOffset(0)
		{
			mInstructions.reserve(500);
		}
	};

	const char* mFileName;
	std::vector<char> mStringData;
	std::vector<unsigned char> mByteCodeData;
	std::vector<FunctionRecord> mFunctions;

public:
	ObjectFileHelper(const char* outputFileName)
		: mFileName(outputFileName)
	{
	}

	void AddFunction(ScriptFunction* func);
	void AddVariable(Variable* var);

	IRInstruction& EmitInstruction(OpCode opCode, size_t rootToken);
	//void EmitLabel();

	void WriteFile();

protected:
	int AddString(Identifier id);
};
