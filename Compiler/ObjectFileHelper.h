#pragma once

#include "Identifier.h"
#include "IRCode.h"

class Variable;
class ScriptFunction;
class Label;
class IRScope;

class ObjectFileHelper
{
protected:
	struct FunctionRecord
	{
		ScriptFunction* mFunction;
		IRScope* mFunctionScope;
		int mNameOffset;
		std::vector<IRInstruction*> mInstructions;
		std::vector<char> mByteCodeData;
		std::vector<int> mStringPtrFixups;

		FunctionRecord(ScriptFunction* func, int nameOffset);
		
		void AddStringPtrFixup(int stackOffset);
	};

	std::vector<FunctionRecord> mFunctions;
	std::vector<char> mStringData;
	std::vector<IRScope*> mScopeStack;
	int mGlobalStackOffset;

public:
	ObjectFileHelper();

	void AddFunction(ScriptFunction* func);
	int AddString(Identifier id);

	IRInstruction* EmitInstruction(OpCode opCode, int rootToken);
	IRInstruction* EmitTerminator(int rootToken);
	IRInstruction* EmitLabel(Label* label, int rootToken);

	void PushScope(int rootToken);
	void PopScope(int rootToken);
	
	void WriteObjectFile(const char* objectFileName);
	void WriteDebugFile(const char* debugFileName);

protected:
	void AssignStackOffsets(FunctionRecord& record);
	void AssignByteCodeOffsets(FunctionRecord& record);

	IRScope* GetCurrentScope();

	friend class DebugWriter;
	friend class ByteCodeWriter;
	friend class ReferenceCollector;
	friend class ByteCodeLocator;
	friend class Optimizer;
	friend class StackPointerMover;
};
