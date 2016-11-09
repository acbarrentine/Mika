#include "stdafx.h"
#include "ObjectFileHelper.h"
#include "ScriptFunction.h"
#include "Compiler.h"
#include "IRCode.h"
#include "FunctionDeclaration.h"
#include "Variable.h"
#include "Type.h"
#include "../MikaVM/MikaArchive.h"
#include "../MikaVM/MikaVM.h"
#include "ByteCodeWriter.h"
#include "ByteCodeLocator.h"
#include "DebugWriter.h"
#include "ReferenceCollector.h"
#include "Optimizer.h"

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

ObjectFileHelper::FunctionRecord::FunctionRecord(ScriptFunction* func)
	: mFunction(func)
	, mStackUsage(0)
{
	Token& tok = GCompiler.GetToken(func->GetRootToken());
	Identifier fileNameStem = GCompiler.GetStemName(tok.GetFileIndex());
	Identifier functionName = func->GetName();
	Identifier combinedName = GCompiler.ComposeIdentifier("%s:%s", fileNameStem.GetString(), functionName.GetString());
	AddString(combinedName);
	mInstructions.reserve(200);
}

int ObjectFileHelper::FunctionRecord::AddString(Identifier id)
{
	size_t len = mStringData.size();
	const char* str = id.GetString();
	mStringData.insert(mStringData.end(), str, str + strlen(str));
	mStringData.push_back('\0');
	return (int)len;
}

void ObjectFileHelper::AddFunction(ScriptFunction* func)
{
	mFunctions.emplace_back(func);
	func->GenCode(*this);

	FunctionRecord& record = mFunctions.back();

	Optimizer optimizer;
	optimizer.OptimizeFunction(record);

	AssignStackOffsets(record);
	AssignByteCodeOffsets(record);

	ByteCodeWriter byteCodeWriter(record);
	byteCodeWriter.WriteFunction();
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

	for (FunctionRecord& record : mFunctions)
	{
		MikaArchiveFunctionHeader header;
		header.mStackSize = record.mStackUsage;
		header.mStringData = std::move(record.mStringData);
		header.mByteData = std::move(record.mByteCodeData);
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

void ObjectFileHelper::AssignStackOffsets(FunctionRecord& record)
{
	// mark variables referenced
	ReferenceCollector collector(record);
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
	record.mStackUsage = stackOffset;
}

void ObjectFileHelper::AssignByteCodeOffsets(FunctionRecord& record)
{
	ByteCodeLocator byteCodeLocator;
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&byteCodeLocator);
	}

	LabelLocator labelLocator;
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&labelLocator);
	}
}
