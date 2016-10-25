#pragma once

class MikaScript;
class MikaArchive
{
public:
	virtual void Serialize(void* v, size_t size) = 0;

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
	bool mFailed;

public:
	MikaReader()
		: mFailed(false)
	{}

	virtual void Serialize(void* v, size_t size);

	template <typename T>
	friend MikaReader& operator<<(MikaReader& ar, std::vector<T>& vec)
	{
		size_t size;
		ar << size;
		vec.resize(size);
		ar.Serialize(&vec[0], size);
		return ar;
	}

	void Process(const char* fileName, MikaScript* script);
	bool Failed() { return mFailed; }
};

struct MikaArchiveFileHeader
{
	unsigned int mMagic;
	unsigned int mNumFunctions;
	unsigned int mStringDataSize;
	unsigned int mByteDataSize;

	friend MikaReader& operator <<(MikaReader& ar, MikaArchiveFileHeader& header)
	{
		ar << header.mMagic;
		ar << header.mNumFunctions;
		ar << header.mStringDataSize;
		ar << header.mByteDataSize;
		return ar;
	}
};

struct MikaArchiveFunctionHeader
{
	unsigned int mNameOffset;
	unsigned int mByteCodeSize;
	unsigned int mStackSize;

	friend MikaReader& operator <<(MikaReader& ar, MikaArchiveFunctionHeader& header)
	{
		ar << header.mNameOffset;
		ar << header.mByteCodeSize;
		ar << header.mStackSize;
		return ar;
	}
};
