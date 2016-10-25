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

	virtual void Serialize(void* v, size_t size)
	{
		mStream.write((char*)v, size);
	}

	template <typename T>
	friend MikaWriter& operator<<(MikaWriter& ar, std::vector<T>& vec)
	{
		size_t size = vec.size();
		ar.Serialize(&size, sizeof(size));
		ar.Serialize(&vec[0], size);
		return ar;
	}

	void Process(const char* fileName)
	{
		mStream.open(fileName, std::ios::out | std::ios::binary);

		if (mStream.good())
		{
			std::vector<char> dummyCharData;
			dummyCharData.resize(100);
			std::vector<unsigned char> dummyByteData;
			dummyByteData.resize(100);

			MikaArchiveFileHeader fileHeader;
			fileHeader.mMagic = 'MIKA';
			fileHeader.mByteDataSize = dummyByteData.size();
			fileHeader.mStringDataSize = dummyCharData.size();
			fileHeader.mNumFunctions = 0;

			*this << fileHeader;

			*this << dummyCharData;
			*this << dummyByteData;
		}
	}

	bool Failed() { return mFailed; }
};

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
