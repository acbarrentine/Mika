#pragma once

class ByteCodeLocator : public IRVisitor
{
protected:
	int mByteCodeOffset;

public:
	ByteCodeLocator(int startOffset)
		: mByteCodeOffset(startOffset)
	{
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}

	void Visit(IRInstruction* op)
	{
		op->SetByteCodeOffset(mByteCodeOffset);
		mByteCodeOffset += op->GetSize();
	}

	void Visit(class IRLabelInstruction* op) override
	{
		op->SetByteCodeOffset(mByteCodeOffset);
	}
	
	void Visit(class IRReturnInstruction* op) override
	{
		op->SetByteCodeOffset(mByteCodeOffset);
		mByteCodeOffset += op->GetSize();
	}

	int GetByteCodeOffset() const
	{
		return mByteCodeOffset;
	}
};

class LabelLocator : public IRVisitor
{
protected:
	int mByteCodeOffset;

public:
	LabelLocator()
		: mByteCodeOffset(0)
	{
	}

	void Visit(IRLabelOperand* op, bool) override
	{
		int targetOffset = op->mLabel->mByteCodeOffset;
		op->SetOffset(targetOffset - mByteCodeOffset);
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}

	void Visit(IRInstruction* op)
	{
		mByteCodeOffset = op->mByteCodeOffset;
		mByteCodeOffset += op->GetSize();

		int numOperands = op->GetNumOperands();
		for (int i = 0; i < numOperands; ++i)
		{
			IROperand* operand = op->mOperands[i];
			operand->Accept(this, op->WritesOperand(i));
		}
	}

	void Visit(class IRLabelInstruction*) override {}
	void Visit(class IRReturnInstruction*) override {}
};
