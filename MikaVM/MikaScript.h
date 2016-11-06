#pragma once

class MikaScript
{
public:
	typedef void(*GlueFunc)();

	union Cell
	{
		int mIntVal;
		double mDblVal;
		void* mPtrVal;
	};

	struct OpCode
	{
		GlueFunc mFunc;
		short mFileIndex;
		short mLineNumber;
		short mNumArgs;
		short mFlags;

		Cell* GetArgs() const;
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
