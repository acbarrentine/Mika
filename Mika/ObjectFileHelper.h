#pragma once

#include "Identifier.h"
class Variable;
class ScriptFunction;

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs) op,
enum OpCode
{
	IllegalInstruction,
#include "..\MikaVM\MikaOpcodes.h"
};

class ObjectFileHelper
{
protected:
	struct FunctionRecord
	{
		ScriptFunction* mFunction;
		int mNameOffset;
		int mByteCodeOffset;
	};

	struct Instruction
	{
		OpCode mOpCode;
		int mNumArgs;
		size_t mRootToken;
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

	void EmitInstruction(OpCode opCode, size_t rootToken);
	//void EmitLabel();

	void WriteFile();

protected:
	int AddString(Identifier id);
};
