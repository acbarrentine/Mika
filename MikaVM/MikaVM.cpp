#include "stdafx.h"
#include "MikaArchive.h"
#include "MikaVM.h"
#include "Glue.h"

MikaVM::MikaVM(int stackSize)
	: mOperands(nullptr)
	, mBasePtr(nullptr)
	, mStackPtr(nullptr)
{
	mStack.insert(mStack.begin(), stackSize, 0);
	
	RegisterGlueFunctions(this);
}

void MikaVM::RegisterGlue(const char* name, GlueFunc func)
{
	mGlueFunctions[name] = func;
}

void MikaVM::Import(const char* fileName)
{
	MikaReader reader;
	reader.Process(fileName, this);
}

void MikaVM::Execute(const char* functionName)
{
	auto& it = mScriptFunctions.find(functionName);
	if (it == mScriptFunctions.end())
	{
		std::cerr << "Function " << functionName << " not found." << std::endl;
		return;
	}

	Function& func = it->second;
	mLoc.Function = func.mName;
	mBasePtr = &mStack[0];
	mStackPtr = mBasePtr + func.mStackSize;

	for (mLoc.PCOffset = 0; mLoc.PCOffset < func.mByteData.size(); )
	{
		MikaVM::Instruction* op = (MikaVM::Instruction*)&func.mByteData[mLoc.PCOffset];
		if (!op->mFunc)
		{
			break;
		}

		mLoc.LineNumber = op->mLineNumber;

		mOperands = op->GetOperands();
		mLoc.PCOffset += op->GetSize();

		op->mFunc(this);
	}

	mBasePtr = nullptr;
	mStackPtr = nullptr;
	mOperands = nullptr;
}

MikaVM::Cell MikaVM::GetOperand(int index)
{
	return mOperands[index];
}

MikaVM::Cell MikaVM::GetFunctionArg(int index)
{
	return mFunctionArgs[index];
}

MikaVM::Cell MikaVM::GetStackValue(int offset)
{
	assert(mBasePtr + offset < mStackPtr);
	Cell* cell = (Cell*)(mBasePtr + offset);
	return *cell;
}

MikaVM::GlueFunc MikaVM::GetGlueFunction(const char* name)
{
	return mGlueFunctions[name];
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
	assert(mBasePtr + stackOffset < mStackPtr);
	Cell* cell = (Cell*)(mBasePtr + stackOffset);
	*cell = value;
}

void MikaVM::SetPCOffset(size_t offset)
{
	mLoc.PCOffset = offset;
}

MikaVM::Location MikaVM::GetLocation() const
{
	return mLoc;
}

void Glue_CopyArgToStack(MikaVM*)
{
}

void Glue_CopyConstantToStack(MikaVM* vm)
{
	MikaVM::Cell stackIndex = vm->GetOperand(0);
	MikaVM::Cell str = vm->GetOperand(1);
	vm->CopyToStack(str, stackIndex.mIntVal);
}

void Glue_CopyStackToStack(MikaVM*)
{
}

void Glue_PushArgument(MikaVM* vm)
{
	MikaVM::Cell stackIndex = vm->GetOperand(0);
	MikaVM::Cell value = vm->GetStackValue(stackIndex.mIntVal);
	vm->PushFunctionArg(value);
}

void Glue_CallNativeFunction(MikaVM* vm)
{
	MikaVM::Cell nameCell = vm->GetOperand(0);
	const char* name = (const char*)nameCell.mPtrVal;
	MikaVM::GlueFunc glue = vm->GetGlueFunction(name);
	if (!glue)
	{
		std::cerr << "Failed to find glue function " << name << "!" << std::endl;
		return;
	}

	glue(vm);

	vm->ResetFunctionArgs();
}

void Glue_CallScriptFunction(MikaVM*)
{
}

void Glue_CopyResultRegister(MikaVM*)
{
}

void Glue_SetResultRegister(MikaVM*)
{
}

void Glue_ConditionalBranch(MikaVM* vm)
{
	MikaVM::Cell cond = vm->GetOperand(0);
	if (!cond.mIntVal)
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

void Glue_AddInt(MikaVM*)
{
}

void Glue_AddFloat(MikaVM*)
{
}

void Glue_SubtractInt(MikaVM*)
{
}

void Glue_SubtractFloat(MikaVM*)
{
}

void Glue_DivideInt(MikaVM*)
{
}

void Glue_DivideFloat(MikaVM*)
{
}

void Glue_MultiplyInt(MikaVM*)
{
}

void Glue_MultiplyFloat(MikaVM*)
{
}

void Glue_AssignInt(MikaVM*)
{
}

void Glue_AssignFloat(MikaVM*)
{
}

void Glue_AssignString(MikaVM*)
{
}

void Glue_EqualsInt(MikaVM*)
{
}

void Glue_EqualsFloat(MikaVM*)
{
}

void Glue_EqualsString(MikaVM*)
{
}

void Glue_LessEqualsInt(MikaVM*)
{
}

void Glue_LessEqualsFloat(MikaVM*)
{
}

void Glue_LessEqualsString(MikaVM*)
{
}

void Glue_GreaterEqualsInt(MikaVM*)
{
}

void Glue_GreaterEqualsFloat(MikaVM*)
{
}

void Glue_GreaterEqualsString(MikaVM*)
{
}

void Glue_LessThanInt(MikaVM* vm)
{
	MikaVM::Cell dest = vm->GetOperand(0);
	MikaVM::Cell lhs = vm->GetOperand(1);
	MikaVM::Cell rhs = vm->GetOperand(2);
	MikaVM::Cell truth(lhs.mIntVal < rhs.mIntVal);
	vm->CopyToStack(truth, dest.mIntVal);
}

void Glue_LessThanFloat(MikaVM*)
{
}

void Glue_LessThanString(MikaVM*)
{
}

void Glue_GreaterThanInt(MikaVM*)
{
}

void Glue_GreaterThanFloat(MikaVM*)
{
}

void Glue_GreaterThanString(MikaVM*)
{
}

void Glue_NotEqualsInt(MikaVM*)
{
}

void Glue_NotEqualsFloat(MikaVM*)
{
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
