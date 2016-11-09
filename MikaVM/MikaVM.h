#pragma once

class MikaVM
{
public:
	typedef void(*GlueFunc)(class MikaVM*);

	struct Function
	{
		const char* mName;
		int mStackSize;
		std::vector<char> mStringData;
		std::vector<unsigned char> mByteData;
	};

	union Cell
	{
		int mIntVal;
		double mDblVal;
		void* mPtrVal;
	};

	struct Instruction
	{
		GlueFunc mFunc;
		short mLineNumber;
		short mNumArgs;
		int mFlags;

		Cell* GetArgs() const;

		int GetSize() const
		{
			return mFunc ? sizeof(Instruction) + (mNumArgs * sizeof(Cell)) : sizeof(Cell);
		}
	};

	struct Location
	{
		const char* Function;
		unsigned int LineNumber;
	};

protected:
	std::map<std::string, Function> mFunctions;
	Cell mConditionRegister;
	Location mLoc;

	static GlueFunc SBuiltInFunctions[];

	friend class MikaReader;

public:
	MikaVM() {}

	void Import(const char* fileName);
	void Execute(const char* functionName);
};
