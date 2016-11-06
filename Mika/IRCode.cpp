#include "stdafx.h"
#include "IRCode.h"
#include "Compiler.h"
#include "../MikaVM/MikaScript.h"


int IRRegisterOperand::SDummyRegister = 0;

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs, numWrites) op, #op, numArgs, numWrites,
IRInstruction::OpCodeData IRInstruction::SOpCodeData[] =
{
	IllegalInstruction, "IllegalInstruction", 0, 0,
#include "..\MikaVM\MikaOpcodes.h"
};

int IRInstruction::GetSize() const
{
	return sizeof(MikaScript::OpCode) + (GetNumOperands() * GCompiler.GetCellSize());
}

const char* IRInstruction::GetName() const
{
	return SOpCodeData[mCode].mName;
}

int IRInstruction::GetNumOperands() const
{
	return SOpCodeData[mCode].mNumArgs;
}

bool IRInstruction::WritesOperand(int index)
{
	return index < SOpCodeData[mCode].mNumWrites;
}

int IRReturnInstruction::GetSize() const
{
	return GCompiler.GetCellSize();
}
