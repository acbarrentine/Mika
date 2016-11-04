#pragma once

union MikaCell
{
	int mIntVal;
	double mDblVal;
	void* mPtrVal;
};

class MikaScript
{
public:
	typedef void(*GlueFunc)();

	struct OpCode
	{
		GlueFunc mFunc;
		unsigned short mFileIndex;
		unsigned short mLineNumber;
		unsigned short mNumArgs;
		unsigned short mFlags;

		MikaCell* GetArgs() const;
		OpCode* Next() const;
	};

protected:
	struct FunctionHeader
	{
		const char* mName;
		OpCode* mByteCode;
	};

	std::vector<char> mStringData;
	std::vector<unsigned char> mByteData;
	std::vector<FunctionHeader> mFunctions;

	friend class MikaReader;
	//friend class MikaWriter;

public:
	MikaScript(const char* fileName);
};
