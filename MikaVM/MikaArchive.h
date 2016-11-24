#pragma once

class MikaVM;
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

	void Process(const char* fileName, MikaVM* script);
	bool Failed() { return mStream.bad(); }
};

template <typename T>
MikaArchive& operator<<(MikaArchive& ar, std::vector<T>& vec)
{
	size_t size = vec.size();
	ar << size;
	vec.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		ar << vec[i];
	}
	return ar;
}

struct MikaArchiveStringFixup
{
	int mStringOffset;
	int mByteCodeOffset;
};

struct MikaArchiveFunctionHeader
{
	unsigned int mNameOffset;
	unsigned int mStackSize;
	std::vector<char> mByteData;
	std::vector<int> mStringFixups;

	friend MikaArchive& operator <<(MikaArchive& ar, MikaArchiveFunctionHeader& header)
	{
		ar << header.mNameOffset;
		ar << header.mStackSize;
		ar << header.mByteData;
		ar << header.mStringFixups;
		return ar;
	}
};

struct MikaArchiveFileHeader
{
	unsigned int mMagic;
	std::vector<char> mStringData;
	std::vector<MikaArchiveFunctionHeader> mFunctions;

	friend MikaArchive& operator <<(MikaArchive& ar, MikaArchiveFileHeader& header)
	{
		ar << header.mMagic;
		ar << header.mStringData;
		ar << header.mFunctions;
		return ar;
	}
};

enum OpCode : int;
struct MikaArchiveInstruction
{
	union
	{
		OpCode mCode;
		void* mPlaceholder;
	};
	short mLineNumber;
	short mNumArgs;
	int mFlags;

	friend MikaArchive& operator<<(MikaArchive& ar, MikaArchiveInstruction& instruction)
	{
		ar << instruction.mPlaceholder;
		ar << instruction.mLineNumber;
		ar << instruction.mNumArgs;
		ar << instruction.mFlags;
		return ar;
	}
};

struct MikaArchiveCell
{
	union
	{
		int mIntVal;
		double mDblVal;
		void* mPtrVal;
	};

	MikaArchiveCell()
	{
		memset(this, 0, sizeof(MikaArchiveCell));
	}

	friend MikaArchive& operator<<(MikaArchive& ar, MikaArchiveCell& cell)
	{
		ar.Serialize(&cell, sizeof(MikaArchiveCell));
		return ar;
	}
};
