#include "stdafx.h"
#include "MikaScript.h"
#include "MikaArchive.h"

MikaScript::Cell* MikaScript::Instruction::GetArgs() const
{
	return mNumArgs ? (Cell*)(this + 1) : nullptr;
}

MikaScript::MikaScript(const char* fileName)
{
	MikaReader reader;
	reader.Process(fileName, this);
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

void Glue_CallFunction(MikaVM*)
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
MikaScript::GlueFunc MikaScript::SBuiltInFunctions[] =
{
	nullptr,
#include "MikaOpcodes.h"
};
