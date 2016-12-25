#include "stdafx.h"
#include "IRCode.h"
#include "Compiler.h"
#include "Label.h"
#include "../MikaVM/MikaArchive.h"

int IRRegisterOperand::SDummyRegister = 0;

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs, numWrites) op, #op, numArgs, numWrites,
IRInstruction::OpCodeData IRInstruction::SOpCodeData[] =
{
	IllegalInstruction, "IllegalInstruction", 0, 0,
#include "../MikaVM/MikaOpcodes.h"
};

void IRVisitor::VisitChildren(IRInstruction* op)
{
	int numOperands = op->GetNumOperands();
	for (int i = 0; i < numOperands; ++i)
	{
		IROperand* operand = op->mOperands[i];
		operand->Accept(this, op->WritesOperand(i));
	}
}

int IRInstruction::GetSize() const
{
	return sizeof(MikaArchiveInstruction) + (GetNumOperands() * sizeof(int*));
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

void IRLabelInstruction::SetByteCodeOffset(int byteCodeOffset)
{
	mByteCodeOffset = byteCodeOffset;
	mLabel->SetByteCodeOffset(byteCodeOffset);
}

int IRReturnInstruction::GetSize() const
{
	return sizeof(int*);
}
