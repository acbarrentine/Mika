#pragma once

class MikaScript;
class MikaArchive
{
public:
	virtual void Serialize(void* v, int size) = 0;

	template <typename T>
	friend MikaArchive& operator <<(MikaArchive& ar, T& o)
	{
		ar.Serialize(&o, sizeof(o));
		return ar;
	}
};

class MikaReader : public MikaArchive
{
protected:
	std::ifstream mStream;

public:
	virtual void Serialize(void* v, int size);

	void Process(const char* fileName, MikaScript* script);
	bool Failed() { return mStream.bad(); }
};

template <typename T>
MikaArchive& operator<<(MikaArchive& ar, std::vector<T>& vec)
{
	int size = vec.size();
	ar << size;
	vec.resize(size);
	for (int i = 0; i < size; ++i)
	{
		ar << vec[i];
	}
	return ar;
}

struct MikaArchiveFunctionHeader
{
	unsigned int mNameOffset;
	unsigned int mByteCodeSize;
	unsigned int mStackSize;

	friend MikaArchive& operator <<(MikaArchive& ar, MikaArchiveFunctionHeader& header)
	{
		ar << header.mNameOffset;
		ar << header.mByteCodeSize;
		ar << header.mStackSize;
		return ar;
	}
};

struct MikaArchiveFileHeader
{
	unsigned int mMagic;
	std::vector<char> mStringData;
	std::vector<unsigned char> mByteData;
	std::vector<MikaArchiveFunctionHeader> mFunctions;

	friend MikaArchive& operator <<(MikaArchive& ar, MikaArchiveFileHeader& header)
	{
		ar << header.mMagic;
		ar << header.mStringData;
		ar << header.mByteData;
		ar << header.mFunctions;
		return ar;
	}
};
