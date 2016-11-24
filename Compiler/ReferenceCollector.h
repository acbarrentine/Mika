#pragma once

#include "IRCode.h"

class ReferenceCollector : public IRVisitor
{
protected:
	ObjectFileHelper& mHelper;
	ObjectFileHelper::FunctionRecord& mRecord;

public:
	ReferenceCollector(ObjectFileHelper& helper, ObjectFileHelper::FunctionRecord& record)
		: mHelper(helper)
		, mRecord(record)
	{
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		op->mVariable->AddRef();
	}

	void Visit(IRFunctionOperand* op, bool) override
	{
		op->mStringOffset =	mHelper.AddString(op->mFunction->GetName());
	}

	void Visit(IRStringOperand* op, bool) override
	{
		op->mStringOffset = mHelper.AddString(op->mValue);
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}

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

class VariableLocator : public IRVisitor
{
protected:
	int mStackOffset;
	int mUsedBytes;

public:
	VariableLocator(int startOffset)
		: mStackOffset(startOffset)
		, mUsedBytes(startOffset)
	{
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		Variable* var = op->mVariable;
		if (!var->IsStackOffsetAssigned())
		{
			Type* varType = var->GetType();
			int size = varType->GetSize();
			if (size < sizeof(int*))
				size = sizeof(int*);
			var->SetStackOffset(mStackOffset);
			mStackOffset += size;
			if (mStackOffset > mUsedBytes)
			{
				mUsedBytes = mStackOffset;
			}
		}
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
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

	int GetUsedBytes() const
	{
		return mUsedBytes;
	}
};

class TempRegisterLocator : public IRVisitor
{
protected:
	int mStackOffset;
	int mUsedBytes;

public:
	TempRegisterLocator(int startOffset)
		: mStackOffset(startOffset)
		, mUsedBytes(startOffset)
	{
	}

	void Visit(IRRegisterOperand* op, bool forWrite) override
	{
		// a temp register gets used twice, once for write
		// and then once for read
		if (forWrite)
		{
			op->mStackOffset = mStackOffset;
			mStackOffset += sizeof(int*);
			if (mStackOffset > mUsedBytes)
			{
				mUsedBytes = mStackOffset;
			}
		}
		else
		{
			mStackOffset -= sizeof(int*);
		}
	}

	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
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

	int GetUsedBytes() const
	{
		return mUsedBytes;
	}
};
