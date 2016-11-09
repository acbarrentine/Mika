#include "stdafx.h"
#include "MikaArchive.h"
#include "MikaVM.h"

void MikaVM::Import(const char* fileName)
{
	MikaReader reader;
	reader.Process(fileName, this);
}

void MikaVM::Execute(const char* functionName)
{
	auto& it = mFunctions.find(functionName);
	if (it == mFunctions.end())
	{
		std::cerr << "Function " << functionName << " not found." << std::endl;
		return;
	}

	Function& func = it->second;
	mLoc.Function = func.mName;

	for (size_t pcOffset = 0; pcOffset < func.mByteData.size(); )
	{
		MikaVM::Instruction* op = (MikaVM::Instruction*)&func.mByteData[pcOffset];
		if (!op->mFunc)
		{
			break;
		}

		mLoc.LineNumber = op->mLineNumber;

		op->mFunc(this);

		pcOffset += op->GetSize();
	}
}

void Glue_CopyArgToStack(MikaVM*)
{
}

void Glue_CopyConstantToStack(MikaVM*)
{
}

void Glue_CopyStackToStack(MikaVM*)
{
}

void Glue_PushArgument(MikaVM*)
{
}

void Glue_CallNativeFunction(MikaVM*)
{
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

void Glue_ConditionalBranch(MikaVM*)
{
}

void Glue_UnconditionalBranch(MikaVM*)
{
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

void Glue_LessThanInt(MikaVM*)
{
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
