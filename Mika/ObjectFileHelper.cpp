#include "stdafx.h"
#include "ObjectFileHelper.h"
#include "ScriptFunction.h"
#include "Compiler.h"
#include "..\MikaVM\MikaArchive.h"


// struct OpCodeData
// {
// 	OpCode mCode;
//  const char* mName;
// 	int mNumArgs;
// };
// #undef MIKA_OPCODE
// #define MIKA_OPCODE(op, numArgs) op, #op, numArgs,
// static OpCodeData SOpCodeData[] =
// {
// 	IllegalInstruction, 0,
// #include "..\MikaVM\MikaOpcodes.h"
// };


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
	mFunctions.emplace_back(func, AddString(func->GetName()));
	func->GenCode(*this);
}

void ObjectFileHelper::AddVariable(Variable* var)
{
	var;
}

IRInstruction& ObjectFileHelper::EmitInstruction(OpCode opCode, size_t rootToken)
{
	FunctionRecord& record = mFunctions.back();
	record.mInstructions.emplace_back(opCode, rootToken);
	return record.mInstructions.back();
}

void ObjectFileHelper::WriteFile()
{
	MikaWriter writer;
	writer.Open(mFileName);
	if (writer.Failed())
	{
		GCompiler.Error("Failed to open output file '%s'", mFileName);
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

int ObjectFileHelper::AddString(Identifier id)
{
	int len = mStringData.size();
	const char* str = id.GetString();
	mStringData.insert(mStringData.end(), str, str + strlen(str));
	mStringData.push_back('\0');
	return len;
}
