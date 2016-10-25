#include "stdafx.h"
#include "ObjectFileHelper.h"
#include "..\MikaVM\MikaArchive.h"



class MikaWriter : public MikaArchive
{
protected:
	std::ofstream mStream;
	bool mFailed;

public:
	MikaWriter()
		: mFailed(false)
	{}

	virtual void Serialize(void* v, size_t size);

	void Process(const char* fileName);
	bool Failed() { return mFailed; }
};


void MikaWriter::Serialize(void* v, size_t size)
{
	mStream.write((char*)v, size);
}

void MikaWriter::Process(const char* fileName)
{
	mStream.open(fileName, std::ios::out | std::ios::binary);

	if (mStream.good())
	{
		MikaArchiveFileHeader fileHeader;
		fileHeader.mMagic = 'MIKA';

		*this << fileHeader;

		std::vector<char> dummyCharData;
		dummyCharData.resize(100);
		std::vector<unsigned char> dummyByteData;
		dummyByteData.resize(100);

		*this << dummyCharData;
		*this << dummyByteData;
	}

}

void ObjectFileHelper::AddFunction(ScriptFunction* func)
{
	func;
}

void ObjectFileHelper::AddVariable(Variable* var)
{
	var;
}

void ObjectFileHelper::WriteFile()
{
	MikaWriter writer;
	writer.Process(mFileName);
}
