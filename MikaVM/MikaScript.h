#pragma once

union MikaCell
{
	int mIntVal;
	double mDblVal;
	void* mPtrVal;
};

class MikaScript
{
protected:
	typedef void(*GlueFunc)();

	struct OpCode
	{
		GlueFunc mFunc;
		unsigned char mFileIndex;
		unsigned char mLineNumber;
		unsigned char mNumArgs;
		unsigned char mFlags;

		MikaCell* GetArgs() const;
		OpCode* Next() const;
	};

	struct FunctionHeader
	{
		const char* mName;
		OpCode* mByteCode;
	};

	std::vector<FunctionHeader> mFunctions;

public:
	MikaScript()
	{
	}

	void ReadScript(const char* fileName);
};
