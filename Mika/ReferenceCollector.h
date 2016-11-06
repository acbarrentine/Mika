#pragma once

#include "IRCode.h"

class ReferenceCollector : public IRVisitor
{
public:
	ReferenceCollector() {}

	void Visit(IRVariableOperand* op, bool) override
	{
		op->mVariable->AddRef();
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

	void Visit(class IRLabel*) override {}
	void Visit(class IRReturnInstruction*) override {}
};

class VariableLocator : public IRVisitor
{
protected:
	int mStackOffset;
	int mCellSize;
	int mUsedBytes;

public:
	VariableLocator(int startOffset)
		: mStackOffset(startOffset)
		, mCellSize(GCompiler.GetCellSize())
		, mUsedBytes(startOffset)
	{
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		Variable* var = op->mVariable;
		if (var->GetRefCount())
		{
			Type* varType = var->GetType();
			int size = varType->GetSize();
			if (size < mCellSize)
				size = mCellSize;
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

	void Visit(class IRLabel*) override {}
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
	int mCellSize;
	int mUsedBytes;

public:
	TempRegisterLocator(int startOffset)
		: mStackOffset(startOffset)
		, mCellSize(GCompiler.GetCellSize())
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
			mStackOffset += mCellSize;
			if (mStackOffset > mUsedBytes)
			{
				mUsedBytes = mStackOffset;
			}
		}
		else
		{
			mStackOffset -= mCellSize;
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

	void Visit(class IRLabel*) override {}
	void Visit(class IRReturnInstruction*) override {}

	int GetUsedBytes() const
	{
		return mUsedBytes;
	}
};
