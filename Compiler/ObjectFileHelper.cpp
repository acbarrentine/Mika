#include "stdafx.h"
#include "ObjectFileHelper.h"
#include "ScriptFunction.h"
#include "Compiler.h"
#include "IRCode.h"
#include "FunctionDeclaration.h"
#include "Variable.h"
#include "Type.h"
#include "Label.h"
#include "../MikaVM/MikaArchive.h"
#include "../MikaVM/MikaVM.h"
#include "ByteCodeWriter.h"
#include "ByteCodeLocator.h"
#include "DebugWriter.h"
#include "ReferenceCollector.h"
#include "Optimizer.h"
#include "Platform.h"

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
		Platform::CreateDirectory(fileName);
		mStream.open(fileName, std::ios::out | std::ios::binary);
	}

	bool Failed() { return mStream.fail(); }
};

ObjectFileHelper::FunctionRecord::FunctionRecord(ScriptFunction* func, int nameOffset)
	: mFunction(func)
	, mFunctionScope(nullptr)
	, mNameOffset(nameOffset)
{
	mInstructions.reserve(200);
}

void ObjectFileHelper::FunctionRecord::AddStringPtrFixup(int stackOffset)
{
	mStringPtrFixups.push_back(stackOffset);
}

ObjectFileHelper::ObjectFileHelper()
	: mGlobalStackOffset(0)
{
}

void ObjectFileHelper::AddFunction(ScriptFunction* func)
{
	mFunctions.emplace_back(func, AddString(func->GetName()));
	func->GenCode(*this);

	FunctionRecord& record = mFunctions.back();

	Optimizer optimizer;
	optimizer.OptimizeFunction(record);

	AssignStackOffsets(record);
	AssignByteCodeOffsets(record);

	ByteCodeWriter byteCodeWriter(record);
	byteCodeWriter.WriteFunction();
}

int ObjectFileHelper::AddString(Identifier id)
{
	size_t len = mStringData.size();
	const char* str = id.GetString();
	mStringData.insert(mStringData.end(), str, str + strlen(str));
	mStringData.push_back('\0');
	return (int)len;
}

IRInstruction* ObjectFileHelper::EmitInstruction(OpCode opCode, int rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(new IRInstruction(opCode, rootToken, GetCurrentScope()));
	return record.mInstructions.back();
}

IRInstruction* ObjectFileHelper::EmitTerminator(int rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(new IRReturnInstruction(rootToken, GetCurrentScope()));
	return record.mInstructions.back();
}

IRInstruction* ObjectFileHelper::EmitLabel(Label* label, int rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(new IRLabelInstruction(label, rootToken, GetCurrentScope()));
	label->SetScope(mScopeStack.back());
	return record.mInstructions.back();
}

void ObjectFileHelper::PushScope(int rootToken)
{
	IRScope* parent = nullptr;
	if (mScopeStack.size())
	{
		parent = mScopeStack.back();
	}
	IRScope* newScope = new IRScope(parent);
	mScopeStack.push_back(newScope);

	if (mScopeStack.size() == 1)
	{
		FunctionRecord& record = mFunctions.back();
		record.mFunctionScope = newScope;
	}

	IRInstruction* pushScope = EmitInstruction(MoveStackPointer, rootToken);
	pushScope->SetOperand(0, new IRStackBytesOperand(false));
}

void ObjectFileHelper::PopScope(int rootToken)
{
	IRInstruction* pushScope = EmitInstruction(MoveStackPointer, rootToken);
	pushScope->SetOperand(0, new IRStackBytesOperand(true));
	mScopeStack.pop_back();
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
	fileHeader.mGlobalStackSize = mGlobalStackOffset;
	fileHeader.mStringData = std::move(mStringData);

	for (FunctionRecord& record : mFunctions)
	{
		MikaArchiveFunctionHeader header;
		header.mNameOffset = record.mNameOffset;
		header.mNumArgs = record.mFunction->GetParameterCount();
		header.mByteData = std::move(record.mByteCodeData);
		header.mStringPtrFixups = std::move(record.mStringPtrFixups);

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
	ReferenceCollector collector(*this, record);
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&collector);
	}

	TempRegisterLocator tempLocator(0, record.mFunctionScope);
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&tempLocator);
	}
	int stackBytesUsed = tempLocator.GetUsedBytes();

	// assign stack locations to used variables
	VariableLocator varLocator(stackBytesUsed, mGlobalStackOffset);
	for (IRInstruction* op : record.mInstructions)
	{
		op->Accept(&varLocator);
	}

	StackPointerMover stackMover;
	stackMover.ScanFunction(record);
}

void ObjectFileHelper::AssignByteCodeOffsets(FunctionRecord& record)
{
	ByteCodeLocator byteCodeLocator(record);
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

IRScope* ObjectFileHelper::GetCurrentScope()
{
	return mScopeStack.empty() ? nullptr : mScopeStack.back();
}
