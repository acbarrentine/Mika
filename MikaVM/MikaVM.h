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
		double mDblVal;
		int mIntVal;
		void* mPtrVal;
		Cell() : mDblVal(0) {}
		Cell(int val) : mIntVal(val) {}
		Cell(double val) : mDblVal(val) {}
		Cell(void* val) : mPtrVal(val) {}
	};

	struct Instruction
	{
		GlueFunc mFunc;
		short mLineNumber;
		short mNumArgs;
		int mFlags;

		Cell* GetOperands() const
		{
			return mNumArgs ? (Cell*)(this + 1) : nullptr;
		}

		int GetSize() const
		{
			return mFunc ? sizeof(Instruction) + (mNumArgs * sizeof(Cell)) : sizeof(Cell);
		}
	};

	struct Location
	{
		const char* Function;
		unsigned int LineNumber;
		Location() : Function(nullptr), LineNumber(0) {}
	};

protected:
	std::map<std::string, Function> mScriptFunctions;
	std::map<std::string, GlueFunc> mGlueFunctions;

	Cell mConditionRegister;
	Cell* mOperands;
	char* mBasePtr;
	char* mStackPtr;
	std::vector<Cell> mFunctionArgs;
	std::vector<char> mStack;
	Location mLoc;

	static GlueFunc SBuiltInFunctions[];

	friend class MikaReader;

public:
	MikaVM(int stackSize = 512);

	void RegisterGlue(const char* name, GlueFunc func);
	void Import(const char* fileName);
	void Execute(const char* functionName);

	Cell GetOperand(int index);
	Cell GetFunctionArg(int index);
	Cell GetStackValue(int offset);
	GlueFunc GetGlueFunction(const char* name);

	void PushFunctionArg(Cell value);
	void CopyToStack(Cell value, int stackOffset);
};
