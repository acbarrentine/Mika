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

		// TODO
		// Include a pointer to a shared GlobalContext object that references
		// any global vars or similar per-script content
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
		Function* Func;
		char* BasePtr;
		char* StackPtr;
		unsigned int LineNumber;
		size_t PCOffset;
		
		Location()
		{
			Reset();
		}

		void Reset()
		{
			Func = nullptr;
			BasePtr = nullptr;
			StackPtr = nullptr;
			LineNumber = 0;
			PCOffset = 0;
		}
	};

protected:
	std::map<std::string, Function> mScriptFunctions;
	std::map<std::string, GlueFunc> mGlueFunctions;

	Cell* mOperands;
	std::vector<Cell> mFunctionArgs;
	std::vector<char> mStack;
	std::vector<Location> mCallFrames;
	Cell mResultRegister;

	static GlueFunc SBuiltInFunctions[];

	friend class MikaReader;

public:
	MikaVM(int stackSize = 512);

	void RegisterGlue(const char* name, GlueFunc func);
	void Import(const char* fileName);
	void Execute(const char* functionName);

	Cell GetOperand(int index);
	Cell GetOperandStackValue(int index);
	Cell GetFunctionArg(int index);
	Cell GetStackValue(int offset);
	GlueFunc GetGlueFunction(const char* functionName);
	Function* GetScriptFunction(const char* functionName);
	void ResetFunctionArgs();

	void PushFunctionArg(Cell value);
	void CopyToStack(Cell value, int stackOffset);
	void SetResultRegister(Cell value);
	Cell GetResultRegister() const;

	void SetPCOffset(size_t offset);
	Location& GetLocation();
	void PushCallFrame(Function* func);
	void PopCallFrame();
};
