#include "stdafx.h"
#include "MikaArchive.h"
#include "MikaVM.h"
#include "Glue.h"

MikaVM::MikaVM(int stackSize)
	: mOperands(nullptr)
{
	mStack.insert(mStack.begin(), stackSize, 0);
	
	RegisterGlueFunctions(this);
}

void MikaVM::RegisterGlue(const char* name, GlueFunc func)
{
	mGlueFunctions[name] = func;
}

void MikaVM::RegisterGlue(std::initializer_list<std::pair<const std::string, GlueFunc>> list)
{
	mGlueFunctions.insert(list);
}

void MikaVM::Import(const char* fileName)
{
	MikaReader reader;
	reader.Process(fileName, this);
}

void MikaVM::Execute(const char* functionName)
{
	Function* func = GetScriptFunction(functionName);
	if (!func)
	{
		std::cerr << "Function " << functionName << " not found." << std::endl;
		return;
	}

	PushCallFrame(func);
	size_t entryFrame = mCallFrames.size();

	while (1)
	{
		// because execution can change program flow, we need to retrieve
		// location on each pass through the loop
		Location& currentLoc = GetLocation();
		Function* currentFunc = currentLoc.Func;

		assert(currentLoc.PCOffset < currentFunc->mByteData.size());

		MikaVM::Instruction* op = (MikaVM::Instruction*)&currentFunc->mByteData[currentLoc.PCOffset];
		if (!op->mFunc)
		{
			// return to previous call frame
			if (mCallFrames.size() > entryFrame)
			{
				PopCallFrame();
				continue;
			}
			break;
		}

		currentLoc.LineNumber = op->mLineNumber;

		mOperands = op->GetOperands();
		currentLoc.PCOffset += op->GetSize();

		op->mFunc(this);
	}

	mCallFrames.pop_back();
	mOperands = nullptr;
}

MikaVM::Cell MikaVM::GetOperand(int index)
{
	return mOperands[index];
}

MikaVM::Cell MikaVM::GetOperandStackValue(int index)
{
	Cell loc = GetOperand(index);
	return GetStackValue(loc.mIntVal);
}

MikaVM::Cell MikaVM::GetFunctionArg(int index)
{
	return mFunctionArgs[index];
}

MikaVM::Cell MikaVM::GetStackValue(int offset)
{
	Location& loc = GetLocation();
	assert(loc.BasePtr + offset < loc.StackPtr);
	Cell* cell = (Cell*)(loc.BasePtr + offset);
	return *cell;
}

MikaVM::GlueFunc MikaVM::GetGlueFunction(const char* functionName)
{
	return mGlueFunctions[functionName];
}

MikaVM::Function* MikaVM::GetScriptFunction(const char* functionName)
{
	const auto& it = mScriptFunctions.find(functionName);
	if (it == mScriptFunctions.end())
	{
		return nullptr;
	}

	return &it->second;
}

void MikaVM::ResetFunctionArgs()
{
	mFunctionArgs.clear();
}

void MikaVM::PushFunctionArg(MikaVM::Cell value)
{
	mFunctionArgs.push_back(value);
}

void MikaVM::CopyToStack(Cell value, int stackOffset)
{
	Location& loc = GetLocation();
	assert(loc.BasePtr + stackOffset < loc.StackPtr);
	Cell* cell = (Cell*)(loc.BasePtr + stackOffset);
	*cell = value;
}

void MikaVM::SetResultRegister(Cell value)
{
	mResultRegister = value;
}

MikaVM::Cell MikaVM::GetResultRegister() const
{
	return mResultRegister;
}

void MikaVM::SetPCOffset(size_t offset)
{
	Location& loc = GetLocation();
	loc.PCOffset = offset;
}

MikaVM::Location& MikaVM::GetLocation()
{
	return mCallFrames.back();
}

void MikaVM::PushCallFrame(Function* func)
{
	size_t stackDepth = mCallFrames.size();
	if (!stackDepth)
	{
		mCallFrames.emplace_back();
		MikaVM::Location& newFrame = GetLocation();

		newFrame.Func = func;
		newFrame.BasePtr = &mStack[0];
		newFrame.StackPtr = newFrame.BasePtr + func->mStackSize;
	}
	else
	{
		mCallFrames.emplace_back();
		MikaVM::Location& currentLoc = mCallFrames[stackDepth - 1];
		MikaVM::Location& newFrame = GetLocation();

		newFrame.Func = func;
		newFrame.BasePtr = currentLoc.StackPtr;
		newFrame.StackPtr = newFrame.BasePtr + func->mStackSize;
	}
}

void MikaVM::PopCallFrame()
{
	mCallFrames.pop_back();
}

void Glue_CopyArgToStack(MikaVM* vm)
{
	MikaVM::Cell stackIndex = vm->GetOperand(0);
	MikaVM::Cell argIndex = vm->GetOperand(1);
	MikaVM::Cell argValue = vm->GetFunctionArg(argIndex.mIntVal);
	vm->CopyToStack(argValue, stackIndex.mIntVal);
}

void Glue_CopyConstantToStack(MikaVM* vm)
{
	MikaVM::Cell stackIndex = vm->GetOperand(0);
	MikaVM::Cell val = vm->GetOperand(1);
	vm->CopyToStack(val, stackIndex.mIntVal);
}

void Glue_CopyStackToStack(MikaVM* vm)
{
	MikaVM::Cell dstStackIndex = vm->GetOperand(0);
	MikaVM::Cell srcStackIndex = vm->GetOperand(1);
	MikaVM::Cell value = vm->GetStackValue(srcStackIndex.mIntVal);
	vm->CopyToStack(value, dstStackIndex.mIntVal);
}

void Glue_PushArgument(MikaVM* vm)
{
	MikaVM::Cell value = vm->GetOperandStackValue(0);
	MikaVM::Cell resetArgs = vm->GetOperand(1);
	if (resetArgs.mIntVal)
	{
		vm->ResetFunctionArgs();
	}
	vm->PushFunctionArg(value);
}

void Glue_CallNativeFunction(MikaVM* vm)
{
	MikaVM::Cell nameCell = vm->GetOperand(0);
	const char* name = (const char*)nameCell.mPtrVal;
	MikaVM::GlueFunc glue = vm->GetGlueFunction(name);
	assert(glue != nullptr);

	glue(vm);
}

void Glue_CallScriptFunction(MikaVM* vm)
{
	// locate the target
	MikaVM::Cell nameCell = vm->GetOperand(0);
	const char* name = (const char*)nameCell.mPtrVal;
	MikaVM::Function* func = vm->GetScriptFunction(name);
	if (!func)
	{
		std::cerr << "Function " << name << " not found." << std::endl;
		return;
	}

	vm->PushCallFrame(func);
}

void Glue_CopyResultRegister(MikaVM* vm)
{
	MikaVM::Cell loc = vm->GetOperand(0);
	MikaVM::Cell value = vm->GetResultRegister();
	vm->CopyToStack(value, loc.mIntVal);
}

void Glue_SetResultRegister(MikaVM* vm)
{
	MikaVM::Cell value = vm->GetOperandStackValue(0);
	vm->SetResultRegister(value);
}

void Glue_ConditionalBranch(MikaVM* vm)
{
	MikaVM::Cell condValue = vm->GetOperandStackValue(0);
	if (!condValue.mIntVal)
	{
		MikaVM::Cell dest = vm->GetOperand(1);
		vm->SetPCOffset(dest.mIntVal);
	}
}

void Glue_UnconditionalBranch(MikaVM* vm)
{
	MikaVM::Cell dest = vm->GetOperand(0);
	vm->SetPCOffset(dest.mIntVal);
}

void Glue_AddInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal + rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_AddFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal + rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_SubtractInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal - rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_SubtractFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal - rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_DivideInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	if (!rhs.mIntVal)
	{
		std::cerr << "Divide by 0!" << std::endl;
		return;
	}
	MikaVM::Cell result(lhs.mIntVal / rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_DivideFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	if (!rhs.mDblVal)
	{
		std::cerr << "Divide by 0!" << std::endl;
		return;
	}
	MikaVM::Cell result(lhs.mDblVal / rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_MultiplyInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal * rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_MultiplyFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal * rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_EqualsInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal == rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_EqualsFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal == rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_EqualsString(MikaVM*)
{
}

void Glue_LessEqualsInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal <= rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_LessEqualsFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal <= rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_LessEqualsString(MikaVM*)
{
}

void Glue_GreaterEqualsInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal >= rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_GreaterEqualsFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal >= rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_GreaterEqualsString(MikaVM*)
{
}

void Glue_LessThanInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal < rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_LessThanFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal < rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_LessThanString(MikaVM*)
{
}

void Glue_GreaterThanInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal > rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_GreaterThanFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal > rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_GreaterThanString(MikaVM*)
{
}

void Glue_NotEqualsInt(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mIntVal != rhs.mIntVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_NotEqualsFloat(MikaVM* vm)
{
	MikaVM::Cell destLoc = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperandStackValue(1);
	MikaVM::Cell rhs = vm->GetOperandStackValue(2);
	MikaVM::Cell result(lhs.mDblVal != rhs.mDblVal);
	vm->CopyToStack(result, destLoc.mIntVal);
}

void Glue_NotEqualsString(MikaVM*)
{
}

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs, numWrites) Glue_ ## op,
MikaVM::GlueFunc MikaVM::SBuiltInFunctions[] =
{
	nullptr,
#include "MikaOpcodes.h"
};
