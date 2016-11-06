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
		std::vector<IRInstruction*> mInstructions;

		FunctionRecord(ScriptFunction* func, int nameOffset)
			: mFunction(func)
			, mNameOffset(nameOffset)
			, mByteCodeOffset(0)
		{
			mInstructions.reserve(200);
		}
	};

	std::vector<char> mStringData;
	std::vector<unsigned char> mByteCodeData;
	std::vector<FunctionRecord> mFunctions;
	int mByteCodeOffset;

public:
	ObjectFileHelper()
		: mByteCodeOffset(0)
	{
	}

	void AddFunction(ScriptFunction* func);
	void AddVariable(Variable* var);

	IRInstruction* EmitInstruction(OpCode opCode, int rootToken);
	void EmitReturn(int rootToken);

	IRLabelInstruction* GenLabel(Identifier name, int rootToken);
	void EmitLabel(IRLabelInstruction* label);

	void WriteObjectFile(const char* objectFileName);
	void WriteDebugFile(const char* debugFileName);

protected:
	int AddString(Identifier id);
	void AssignStackOffsets(FunctionRecord& record);
	void AssignByteCodeOffsets(FunctionRecord& record);

	friend class DebugWriter;
	friend class ByteCodeWriter;
};
