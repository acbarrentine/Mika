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
		int mStackUsage;
		std::vector<IRInstruction*> mInstructions;
		std::vector<char> mStringData;
		std::vector<unsigned char> mByteCodeData;
		std::vector<int> mStringFixups;

		FunctionRecord(ScriptFunction* func);
		int AddString(Identifier id);
		void AddStringFixup(int stackOffset);
	};

	std::vector<FunctionRecord> mFunctions;

public:
	ObjectFileHelper()
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
	void AssignStackOffsets(FunctionRecord& record);
	void AssignByteCodeOffsets(FunctionRecord& record);

	friend class DebugWriter;
	friend class ByteCodeWriter;
	friend class ReferenceCollector;
	friend class ByteCodeLocator;
	friend class Optimizer;
};
