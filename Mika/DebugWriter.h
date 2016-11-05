#pragma once

#include "IRCode.h"

class DebugWriter : public IRVisitor
{
protected:
	std::ofstream mStream;

public:
	DebugWriter() {}
	void Open(const char* fileName)
	{
		mStream.open(fileName, std::ios::out);
	}

	bool Failed() { return mStream.bad(); }

	void Visit(IROperand* op) override
	{
		op->Accept(this);
	}

	void Visit(IRFunctionOperand* op) override
	{
		mStream << op->mFunction->GetName().GetString();
	}

	void Visit(IRVariableOperand* op) override
	{
		op;
	}

	void Visit(IRRegisterOperand* op)
	{
		mStream << "%" << op->mTempRegister;
	}

	void Visit(IRLabelOperand* op)
	{
		mStream << std::setfill('0') << std::setw(8) << std::setbase(16) << op->mByteCodeOffset;
	}

	void Visit(IRIntOperand* op)
	{
		mStream << op->mValue;
	}

	void Visit(IRFloatOperand* op)
	{
		mStream << op->mValue;
	}

	void Visit(IRStringOperand* op)
	{
		mStream << "\"" << op->mValue.GetString() << "\"";
	}

	void Visit(IRInstruction* op)
	{
		mStream << "\t" << std::setfill('0') << std::setw(8) << std::setbase(16) << op->mByteCodeOffset << "\t";

		OpCode code = op->mCode;
		mStream << ((code == IllegalInstruction) ? "Return" : IRInstruction::SOpCodeData[code].mName);

		for (int i = 0; i < IRInstruction::SOpCodeData[code].mNumArgs; ++i)
		{
			IROperand* operand = op->mOperands[i];
			if (i)
				mStream << ", ";
			else
				mStream << " ";

			operand->Accept(this);
		}

		mStream << std::endl;
	}

	void WriteFunction(ObjectFileHelper::FunctionRecord& record)
	{
		ScriptFunction* func = record.mFunction;
		mStream << func->GetName().GetString() << std::endl;

		for (IRInstruction& op : record.mInstructions)
		{
			Visit(&op);
		}

		mStream << std::endl;
	}
};


