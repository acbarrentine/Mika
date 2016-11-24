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

	void Visit(IRFunctionOperand* op, bool) override
	{
		mStream << op->mFunction->GetName().GetString();
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		Variable* var = op->mVariable;
		if (var->GetIsGlobal())
			mStream << "g";
		else
			mStream << "%";
		mStream << std::setbase(10) << var->GetStackOffset() << " (" << var->GetName().GetString() <<  ")";
	}

	void Visit(IRRegisterOperand* op, bool) override
	{
		mStream << "%" << std::setbase(10) << op->mStackOffset;
	}

	void Visit(IRLabelOperand* op, bool) override
	{
		mStream << op->mLabel->mLabel.GetString() << " (0x" << std::setbase(16) << op->mByteCodeOffset << ")";
	}

	void Visit(IRIntOperand* op, bool) override
	{
		mStream << std::setbase(10) << op->mValue;
	}

	void Visit(IRFloatOperand* op, bool) override
	{
		mStream << std::fixed << std::setprecision(4) << op->mValue;
	}

	void Visit(IRStringOperand* op, bool) override
	{
		mStream << "\"" << op->mValue.GetString() << "\"";
	}

	void Visit(IRInstruction* op) override
	{
		mStream << "\t" << std::setfill('0') << std::setw(8) << std::setbase(16)
			<< op->mByteCodeOffset << "\t" << op->GetName();

		int numOperands = op->GetNumOperands();
		for (int i = 0; i < numOperands; ++i)
		{
			IROperand* operand = op->mOperands[i];
			if (i)
				mStream << ", ";
			else
				mStream << " ";

			operand->Accept(this, op->WritesOperand(i));
		}

		mStream << std::endl;
	}

	void Visit(class IRLabelInstruction* op) override
	{
		mStream << op->mLabel.GetString() << ":" << std::endl;
	}

	void Visit(class IRReturnInstruction* op) override
	{
		mStream << "\t" << std::setfill('0') << std::setw(8) << std::setbase(16)
			<< op->mByteCodeOffset << "\t" << "Return" << std::endl;
	}

	void WriteFunction(ObjectFileHelper::FunctionRecord& record)
	{
		ScriptFunction* func = record.mFunction;
		mStream << func->GetName().GetString() << std::endl;

		for (IRInstruction* op : record.mInstructions)
		{
			op->Accept(this);
		}

		mStream << std::endl;
	}
};


