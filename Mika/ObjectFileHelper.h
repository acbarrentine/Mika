#pragma once

#include "Identifier.h"
#include "IRCode.h"

class Variable;
class ScriptFunction;

class Label
{
protected:
	int mByteCodeOffset;

public:
	Label() : mByteCodeOffset(0) {}
	void SetByteCodeOffset(int offset) { mByteCodeOffset = offset; }
	int GetByteCodeOffset() const { return mByteCodeOffset; }
};

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

	std::vector<char> mStringData;
	std::vector<unsigned char> mByteCodeData;
	std::vector<FunctionRecord> mFunctions;
	std::vector<Label> mLabels;
	int mByteCodeOffset;

public:
	ObjectFileHelper()
		: mByteCodeOffset(0)
	{
	}

	void AddFunction(ScriptFunction* func);
	void AddVariable(Variable* var);

	IRInstruction& EmitInstruction(OpCode opCode, size_t rootToken);
	int GetByteCodeOffset() const { return mByteCodeOffset; }

	void WriteObjectFile(const char* objectFileName);
	void WriteDebugFile(const char* debugFileName);

protected:
	int AddString(Identifier id);

	friend class DebugWriter;
};
