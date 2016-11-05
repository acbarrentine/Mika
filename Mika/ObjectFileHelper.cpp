#include "stdafx.h"
#include "ObjectFileHelper.h"
#include "ScriptFunction.h"
#include "Compiler.h"
#include "IRCode.h"
#include "FunctionDeclaration.h"
#include "DebugWriter.h"
#include "..\MikaVM\MikaArchive.h"


class MikaWriter : public MikaArchive
{
protected:
	std::ofstream mStream;

public:
	virtual void Serialize(void* v, size_t size)
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
	mByteCodeOffset = 0;

	mFunctions.emplace_back(func, AddString(func->GetName()));
	func->GenCode(*this);

	//FunctionRecord& record = mFunctions.back();
	
	// optimize
	// assign stack addresses
	// byte code
}

void ObjectFileHelper::AddVariable(Variable* var)
{
	var;
}

IRInstruction& ObjectFileHelper::EmitInstruction(OpCode opCode, size_t rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(opCode, rootToken, mByteCodeOffset);
	mByteCodeOffset += record.mInstructions.back().GetSize();
	return record.mInstructions.back();
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
	fileHeader.mByteData.resize(100);
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
	int len = mStringData.size();
	const char* str = id.GetString();
	mStringData.insert(mStringData.end(), str, str + strlen(str));
	mStringData.push_back('\0');
	return len;
}
