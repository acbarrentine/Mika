#pragma once

class MikaScript
{
public:
	typedef void(*GlueFunc)(class MikaVM*);

	union Cell
	{
		int mIntVal;
		double mDblVal;
		void* mPtrVal;
	};

	struct Instruction
	{
		GlueFunc mFunc;
		short mFileIndex;
		short mLineNumber;
		short mNumArgs;
		short mFlags;

		Cell* GetArgs() const;
		
		int GetSize() const
		{
			return mFunc ? sizeof(Instruction) + (mNumArgs * sizeof(Cell)) : sizeof(Cell);
		}
	};

	static GlueFunc SBuiltInFunctions[];

protected:
	struct FunctionHeader
	{
		const char* mName;
		int mByteCodeOffset;
		int mStackSize;
	};

	std::vector<char> mStringData;
	std::vector<unsigned char> mByteData;
	std::vector<FunctionHeader> mFunctions;

	friend class MikaReader;

public:
	MikaScript(const char* fileName);
};
