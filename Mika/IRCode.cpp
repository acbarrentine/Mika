#include "stdafx.h"
#include "IRCode.h"
#include "Compiler.h"
#include "FunctionDeclaration.h"
#include "ObjectFileHelper.h"
#include "../MikaVM/MikaScript.h"
#include <iomanip>


int IRRegisterOperand::SDummyRegister = 0;

struct OpCodeData
{
	OpCode mCode;
	const char* mName;
	int mNumArgs;
};
#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs) op, #op, numArgs,
static OpCodeData SOpCodeData[] =
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

void IRFunctionOperand::DebugSerialize(std::ostream& stream)
{
	stream << mFunction->GetName().GetString();
}

void IRRegisterOperand::DebugSerialize(std::ostream& stream)
{
	stream << "%" << mTempRegister;
}

void IRLabelOperand::DebugSerialize(std::ostream& stream)
{
	stream << std::setfill('0') << std::setw(8) << std::setbase(16) << mByteCodeOffset;
}

void IRIntOperand::DebugSerialize(std::ostream& stream)
{
	stream << mValue;
}

void IRFloatOperand::DebugSerialize(std::ostream& stream)
{
	stream << mValue;
}

void IRStringOperand::DebugSerialize(std::ostream& stream)
{
	stream << mValue;
}

std::ostream& operator<<(std::ostream& stream, IRInstruction& op)
{
	stream << "\t" << std::setfill('0') << std::setw(8) << std::setbase(16) << op.mByteCodeOffset << "\t";
	stream << ((op.mCode == IllegalInstruction) ? "Return" : SOpCodeData[op.mCode].mName);

	for (int i = 0; i < SOpCodeData[op.mCode].mNumArgs; ++i)
	{
		IROperand* operand = op.mOperands[i];
		if (i)
			stream << ", ";
		else
			stream << " ";

		operand->DebugSerialize(stream);
	}
	return stream;
}