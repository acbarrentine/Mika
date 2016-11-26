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
		VisitChildren(op);
	}

	void Visit(class IRLabelInstruction*) override {}
	void Visit(class IRReturnInstruction*) override {}
};

class VariableLocator : public IRVisitor
{
protected:
	int mStackOffset;
	int& mGlobalStackOffset;
	int mUsedBytes;

public:
	VariableLocator(int& initialGlobalOffset)
		: mStackOffset(0)
		, mGlobalStackOffset(initialGlobalOffset)
		, mUsedBytes(0)
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
			if (var->GetIsGlobal())
			{
				var->SetStackOffset(mGlobalStackOffset);
				mGlobalStackOffset += size;
			}
			else
			{
				var->SetStackOffset(mStackOffset);
				mStackOffset += size;
				if (mStackOffset > mUsedBytes)
				{
					mUsedBytes = mStackOffset;
				}
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
		VisitChildren(op);
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
	std::vector<int> mFreeRegisters;
	int mStackOffset;

public:
	TempRegisterLocator(int startOffset)
		: mStackOffset(startOffset)
	{
	}

	void Visit(IRRegisterOperand* op, bool forWrite) override
	{
		// a temp register gets used twice, once for write
		// and then once for read
		if (forWrite)
		{
			if (mFreeRegisters.size())
			{
				op->mStackOffset = mFreeRegisters.back();
				mFreeRegisters.pop_back();
			}
			else
			{
				op->mStackOffset = mStackOffset;
				mStackOffset += sizeof(int*);
			}
		}
		else
		{
			mFreeRegisters.push_back(op->mStackOffset);
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
		VisitChildren(op);
	}

	void Visit(class IRLabelInstruction*) override {}
	void Visit(class IRReturnInstruction*) override {}

	int GetUsedBytes() const
	{
		return mStackOffset;
	}
};
