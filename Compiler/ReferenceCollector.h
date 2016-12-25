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
	void Visit(IRStackBytesOperand*, bool) override {}

	void Visit(IRInstruction* op) override
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

public:
	VariableLocator(int& initialLocalOffset, int& initialGlobalOffset)
		: mStackOffset(initialLocalOffset)
		, mGlobalStackOffset(initialGlobalOffset)
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
			}
		}
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
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

class TempRegisterLocator : public IRVisitor
{
protected:
	std::vector<int> mFreeRegisters;
	std::map<int, int> mUsedRegisters;
	int mStackOffset;

public:
	TempRegisterLocator()
		: mStackOffset(0)
	{
	}

	void Visit(IRRegisterOperand* op, bool forWrite) override
	{
		if (forWrite)
		{
			if (op->mStackOffset < 0)
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

				// set the read count so we know when to release it
				mUsedRegisters[op->mStackOffset] = op->GetReadCount();
			}
		}
		else
		{
			int stackOffset = op->mStackOffset;
			int readCount = mUsedRegisters[stackOffset];
			--readCount;
			if (!readCount)
			{
				mUsedRegisters.erase(stackOffset);
				mFreeRegisters.push_back(stackOffset);
			}
			else
			{
				mUsedRegisters[stackOffset] = readCount;
			}
		}
	}

	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
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

	int GetUsedBytes() const
	{
		return mStackOffset;
	}
};

class StackPointerMover : public IRVisitor
{
protected:
	int mStackBytes;

	struct AddOperation
	{
		IRInstruction* mAddOp;
		int mInitialStackBytes;
		AddOperation(IRInstruction* addOp, int stackBytes) : mAddOp(addOp), mInitialStackBytes(stackBytes) {}
	};
	std::vector<AddOperation> mAddOperations;

public:
	StackPointerMover(int baseLine)
		: mStackBytes(baseLine)
	{
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		int stackOffset = op->mVariable->GetStackOffset() + sizeof(int*);
		if (stackOffset > mStackBytes)
			mStackBytes = stackOffset;
	}

	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}
	void Visit(IRStackBytesOperand*, bool) override {}

	void Visit(IRInstruction* op) override
	{
		if (op->mCode == MoveStackPointer)
		{
			IRStackBytesOperand* amountOp = (IRStackBytesOperand*)op->mOperands[0];
			if (amountOp->mSubtract)
			{
				assert(mAddOperations.size());
				AddOperation& add = mAddOperations.back();
				mAddOperations.pop_back();

				int delta = mStackBytes - add.mInitialStackBytes;
				if (delta)
				{
					IRStackBytesOperand* addAmountOp = (IRStackBytesOperand*)add.mAddOp->mOperands[0];
					addAmountOp->SetStackBytes(delta);
					amountOp->SetStackBytes(-delta);
				}
				else
				{
					op->Remove();
					add.mAddOp->Remove();
				}
			}
			else
			{
				mAddOperations.emplace_back(op, mStackBytes);
			}
		}
		else
		{
			VisitChildren(op);
		}
	}

	void Visit(class IRLabelInstruction*) override {}
	void Visit(class IRReturnInstruction*) override {}
};
