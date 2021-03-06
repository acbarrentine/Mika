#pragma once

class MikaVM
{
public:
	typedef void(*GlueFunc)(class MikaVM*);

	struct GlobalContext
	{
		std::vector<char> mStack;
		std::vector<char> mStringData;
	};

	struct Function
	{
		const char* mName;
		GlobalContext* mGlobalContext;
		std::vector<char> mByteData;
	};

	struct StackIndex
	{
		int mOffset;
		bool mGlobal;
		StackIndex(int offset, bool global) : mOffset(offset), mGlobal(global) {}
	};

	union Cell
	{
		float mFltVal;
		int mIntVal;
		const char* mStrVal;
		void* mPtrVal;
		StackIndex mStackIndex;
		Cell() : mFltVal(0) {}
		Cell(int val) : mIntVal(val) {}
		Cell(float val) : mFltVal(val) {}
		Cell(const char* val) : mStrVal(val) {}
		Cell(void* val) : mPtrVal(val) {}
		Cell(int offset, bool global) : mStackIndex(offset, global) {}
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
	
	typedef void(*DestructorFunc)(void*);
	struct Destructor
	{
		void* Addr;
		DestructorFunc Func;
		Destructor(void* addr, DestructorFunc func) : Addr(addr), Func(func) {}
	};

	struct Location
	{
		Function* Func;
		char* BasePtr;
		char* StackPtr;
		unsigned int LineNumber;
		size_t PCOffset;
		std::vector<Destructor> DestructorsToRun;
		
		Location()
		{
			Reset();
		}

		~Location()
		{
			for (Destructor& destructor : DestructorsToRun)
			{
				destructor.Func(destructor.Addr);
			}
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
	typedef std::map<std::string, Function> ScriptFunctionMap;
	typedef std::map<std::string, GlueFunc> GlueMap;
	
	ScriptFunctionMap mScriptFunctions;
	GlueMap mGlueFunctions;
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
	void RegisterGlue(std::initializer_list<std::pair<const std::string, GlueFunc>> list);
	void Import(const char* fileName);
	void Execute(const char* functionName);
	void Reset();

	Cell GetOperand(int index);
	Cell GetOperandStackValue(int index);
	Cell GetFunctionArg(int index);
	Cell GetStackValue(StackIndex index);
	GlueFunc GetGlueFunction(const char* functionName);
	Function* GetScriptFunction(const char* functionName);
	void ResetFunctionArgs();
	void MoveStackPointer(int numBytes);

	void PushFunctionArg(Cell value);
	void SetStackValue(Cell value, StackIndex index);
	void SetResultRegister(Cell value);
	Cell GetResultRegister() const;

	void SetPCOffset(size_t offset);
	Location& GetLocation();
	void PushCallFrame(Function* func);
	void PopCallFrame();

	void RegisterDestructor(void* addr, DestructorFunc func);
};
