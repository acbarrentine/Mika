#pragma once

class ByteCodeLocator : public IRVisitor
{
protected:
	ObjectFileHelper::FunctionRecord& mRecord;
	int mByteCodeOffset;

public:
	ByteCodeLocator(ObjectFileHelper::FunctionRecord& record)
		: mRecord(record)
		, mByteCodeOffset(0)
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
		mRecord.AddStringFixup(mByteCodeOffset);
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
		mRecord.AddStringFixup(mByteCodeOffset);
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}
	void Visit(IRStackBytesOperand*, bool) override
	{
		mByteCodeOffset += sizeof(MikaArchiveCell);
	}

	void Visit(IRInstruction* op) override
	{
		int targetOffset = mByteCodeOffset + op->GetSize();
		op->SetByteCodeOffset(mByteCodeOffset);
		mByteCodeOffset += sizeof(MikaArchiveInstruction);
		
		VisitChildren(op);

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
		op->SetOffset(op->mLabel->GetByteCodeOffset());
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}
	void Visit(IRStackBytesOperand*, bool) override {}

	void Visit(IRInstruction* op) override
	{
		VisitChildren(op);
	}

	void Visit(class IRLabelInstruction*) override {}
	void Visit(class IRReturnInstruction*) override {}
};
