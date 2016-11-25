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
		int mStackUsage;
		std::vector<IRInstruction*> mInstructions;
		std::vector<char> mByteCodeData;
		std::vector<int> mStringFixups;

		FunctionRecord(ScriptFunction* func, int nameOffset);
		void AddStringFixup(int stackOffset);
	};

	std::vector<FunctionRecord> mFunctions;
	std::vector<char> mStringData;
	int mGlobalStackOffset;

public:
	ObjectFileHelper()
		: mGlobalStackOffset(0)
	{
	}

	void AddFunction(ScriptFunction* func);
	int AddString(Identifier id);

	IRInstruction* EmitInstruction(OpCode opCode, int rootToken);
	void EmitReturn(int rootToken);

	IRLabelInstruction* GenLabel(Identifier name, int rootToken);
	void EmitLabel(IRLabelInstruction* label);

	void WriteObjectFile(const char* objectFileName);
	void WriteDebugFile(const char* debugFileName);

protected:
	void AssignStackOffsets(FunctionRecord& record);
	void AssignByteCodeOffsets(FunctionRecord& record);

	friend class DebugWriter;
	friend class ByteCodeWriter;
	friend class ReferenceCollector;
	friend class ByteCodeLocator;
	friend class Optimizer;
};
