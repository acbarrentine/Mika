#pragma once

struct MikaFileHeader
{
	unsigned int mMagic;
	unsigned int mNumFunctions;
	unsigned int mStringDataSize;
	unsigned int mByteDataSize;

	friend std::ifstream& operator >>(std::ifstream& ar, MikaFileHeader& header)
	{
		ar >> header.mMagic;
		ar >> header.mNumFunctions;
		ar >> header.mStringDataSize;
		ar >> header.mByteDataSize;

		return ar;
	}
};

class MikaReader
{
protected:
	std::ifstream mStream;
	bool mFailed;

public:
	MikaReader()
		: mFailed(false)
	{}

	template <typename T>
	MikaReader& operator >>(T& o)
	{
		mStream >> o;
		return *this;
	}

	void Open(const char* fileName);
	bool Failed() { return mFailed; }
};
