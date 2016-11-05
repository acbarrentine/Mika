#include "stdafx.h"
#include "IRCode.h"
#include "Compiler.h"
#include "../MikaVM/MikaScript.h"


int IRRegisterOperand::SDummyRegister = 0;

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs) op, #op, numArgs,
IRInstruction::OpCodeData IRInstruction::SOpCodeData[] =
{
	IllegalInstruction, "IllegalInstruction", 0,
#include "..\MikaVM\MikaOpcodes.h"
};

int IRInstruction::GetSize() const
{
	if (mCode == IllegalInstruction)
	{
		return GCompiler.GetCellSize();
	}
	else
	{
		return sizeof(MikaScript::OpCode) + SOpCodeData[mCode].mNumArgs * GCompiler.GetCellSize();
	}
}
