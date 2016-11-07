#include "stdafx.h"
#include "ObjectFileHelper.h"
#include "ScriptFunction.h"
#include "Compiler.h"
#include "IRCode.h"
#include "FunctionDeclaration.h"
#include "Variable.h"
#include "Type.h"
#include "DebugWriter.h"
#include "ReferenceCollector.h"
#include "ByteCodeLocator.h"
#include "../MikaVM/MikaArchive.h"
#include "../MikaVM/MikaScript.h"
#include "ByteCodeWriter.h"

class MikaWriter : public MikaArchive
{
protected:
	std::ofstream mStream;

public:
	virtual void Serialize(void* v, int size)
	{
		mStream.write((char*)v, size);
	}

	void Open(const char* fileName)
	{
		mStream.open(fileName, std::ios::out | std::ios::binary);
	}

	bool Failed() { return mStream.bad(); }
};

void ObjectFileHelper::AddFunction(ScriptFunction* func)
{
	mFunctions.emplace_back(func, AddString(func->GetName()));
	func->GenCode(*this);

	FunctionRecord& record = mFunctions.back();

	// TODO: optimize
	
	AssignStackOffsets(record);
	AssignByteCodeOffsets(record);

	ByteCodeWriter byteCodeWriter(mByteCodeData);
	byteCodeWriter.WriteFunction(record);
}

void ObjectFileHelper::AddVariable(Variable* var)
{
	var;
}

IRInstruction* ObjectFileHelper::EmitInstruction(OpCode opCode, int rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(new IRInstruction(opCode, rootToken));
	return record.mInstructions.back();
}

void ObjectFileHelper::EmitReturn(int rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(new IRReturnInstruction(rootToken));
}

IRLabelInstruction* ObjectFileHelper::GenLabel(Identifier name, int rootToken)
{
	return new IRLabelInstruction(name, rootToken);
}

void ObjectFileHelper::EmitLabel(IRLabelInstruction* label)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.push_back(label);
}


void ObjectFileHelper::WriteObjectFile(const char* objectFileName)
{
	MikaWriter writer;
	writer.Open(objectFileName);
	if (writer.Failed())
	{
		GCompiler.Error("Failed to open output file '%s'", objectFileName);
		return;
	}
	
	MikaArchiveFileHeader fileHeader;
	fileHeader.mMagic = 'MIKA';
	fileHeader.mByteData = std::move(mByteCodeData);
	fileHeader.mStringData = std::move(mStringData);

	for (FunctionRecord& record : mFunctions)
	{
		MikaArchiveFunctionHeader header;
		header.mByteCodeSize = 0;
		header.mNameOffset = record.mNameOffset;
		header.mStackSize = 0;
		fileHeader.mFunctions.push_back(header);
	}
	writer << fileHeader;
}

void ObjectFileHelper::WriteDebugFile(const char* debugFileName)
{
	DebugWriter writer;
	writer.Open(debugFileName);
	if (writer.Failed())
	{
		GCompiler.Error("Failed to open debug output file '%s'", debugFileName);
		return;
	}

	for (FunctionRecord& record : mFunctions)
	{
		writer.WriteFunction(record);
	}
}

int ObjectFileHelper::AddString(Identifier id)
{
	size_t len = mStringData.size();
	const char* str = id.GetString();
	mStringData.insert(mStringData.end(), str, str + strlen(str));
	mStringData.push_back('\0');
	return (int)len;
}

void ObjectFileHelper::AssignStackOffsets(FunctionRecord& record)
{
	// mark variables referenced
	ReferenceCollector collector;
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&collector);
	}

	// assign stack locations to used variables
	int stackOffset = 0;
	VariableLocator varLocator(stackOffset);
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&varLocator);
	}
	stackOffset = varLocator.GetUsedBytes();

	TempRegisterLocator tempLocator(stackOffset);
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&tempLocator);
	}
	stackOffset = tempLocator.GetUsedBytes();
}

void ObjectFileHelper::AssignByteCodeOffsets(FunctionRecord& record)
{
	record.mByteCodeOffset = mByteCodeOffset;

	ByteCodeLocator byteCodeLocator(mByteCodeOffset);
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&byteCodeLocator);
	}
	mByteCodeOffset = byteCodeLocator.GetByteCodeOffset();

	LabelLocator labelLocator;
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&labelLocator);
	}
}
