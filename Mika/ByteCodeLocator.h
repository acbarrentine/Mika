#pragma once

class ByteCodeLocator : public IRVisitor
{
protected:
	int mByteCodeOffset;

public:
	ByteCodeLocator()
		: mByteCodeOffset(0)
	{
	}

	void Visit(IRRegisterOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRVariableOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRFunctionOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRLabelOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRIntOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRFloatOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRStringOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}

	void Visit(IRInstruction* op)
	{
		int targetOffset = mByteCodeOffset + op->GetSize();
		op->SetByteCodeOffset(mByteCodeOffset);
		mByteCodeOffset += sizeof(MikaArchiveInstruction);
		
		int numOperands = op->GetNumOperands();
		for (int i = 0; i < numOperands; ++i)
		{
			IROperand* operand = op->mOperands[i];
			operand->Accept(this, op->WritesOperand(i));
		}

		// make sure the operands pushed the offset to the right place
		assert(mByteCodeOffset == targetOffset);
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
public:
	LabelLocator()
	{
	}

	void Visit(IRLabelOperand* op, bool) override
	{
		op->SetOffset(op->mLabel->mByteCodeOffset);
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}

	void Visit(IRInstruction* op)
	{
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
