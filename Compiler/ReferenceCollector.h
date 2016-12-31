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
	IRScope* mCurrentScope;

public:
	VariableLocator(int& initialLocalOffset, int& initialGlobalOffset)
		: mStackOffset(initialLocalOffset)
		, mGlobalStackOffset(initialGlobalOffset)
		, mCurrentScope(nullptr)
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
				mCurrentScope->SetStackOffset(mStackOffset);
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
		mCurrentScope = op->GetScope();
		VisitChildren(op);
	}

	void Visit(IRLabelInstruction* op) override
	{
		mCurrentScope = op->GetScope();
	}
	
	void Visit(IRReturnInstruction* op) override
	{
		mCurrentScope = op->GetScope();
	}
};

class TempRegisterLocator : public IRVisitor
{
protected:
	std::vector<int> mFreeRegisters;
	std::map<int, int> mUsedRegisters;
	int mStackOffset;
	int mInitialOffset;
	IRScope* mFunctionScope;

public:
	TempRegisterLocator(int initialOffset, IRScope* functionScope)
		: mStackOffset(initialOffset)
		, mInitialOffset(initialOffset)
		, mFunctionScope(functionScope)
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
					mFunctionScope->SetStackOffset(mStackOffset);
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

	void Visit(IRLabelInstruction*) override {}
	void Visit(IRReturnInstruction*) override {}

	int GetUsedBytes() const
	{
		return mStackOffset - mInitialOffset;
	}
};

class StackPointerMover : public IRVisitor
{
protected:
	int mStackBytes;
	std::vector<int> mPushes;

public:
	StackPointerMover()
		: mStackBytes(0)
	{
	}

	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}
	void Visit(IRStackBytesOperand*, bool) override {}

	void Visit(IRInstruction* op) override
	{
		switch (op->mCode)
		{
			case MoveStackPointer:
			{
				IRStackBytesOperand* amountOp = (IRStackBytesOperand*)op->mOperands[0];
				int delta;

				if (amountOp->mSubtract)
				{
					delta = -1 * mPushes.back();
					mPushes.pop_back();
					assert(delta <= 0);
					mStackBytes -= delta;
				}
				else
				{
					IRScope* scope = op->GetScope();
					int newStackBytes = scope->GetStackBytes();

					delta = newStackBytes - mStackBytes;
					mStackBytes = newStackBytes;
					assert(delta >= 0);
					mPushes.push_back(delta);
				}
				
				if (delta)
				{
					amountOp->SetStackBytes(delta);
				}
				else
				{
					op->Remove();
				}
				break;
			}
				
			case ConditionalBranch:
			{
				IRLabelOperand* targetLabel = (IRLabelOperand*)op->mOperands[1];
				IRStackBytesOperand* amountOp = (IRStackBytesOperand*)op->mOperands[2];
				IRScope* thisScope = op->GetScope();
				int currentStackBytes = thisScope->GetStackBytes();
				int targetStackBytes = targetLabel->GetTargetStackBytes();
				int delta = targetStackBytes - currentStackBytes;
				// doesn't make sense to jump to a place with more stack usage than the current point
				//assert(delta <= 0);
				amountOp->SetStackBytes(delta);
				break;
			}
				
			case UnconditionalBranch:
			{
				IRLabelOperand* targetLabel = (IRLabelOperand*)op->mOperands[0];
				IRStackBytesOperand* amountOp = (IRStackBytesOperand*)op->mOperands[1];
				IRScope* thisScope = op->GetScope();
				int currentStackBytes = thisScope->GetStackBytes();
				int targetStackBytes = targetLabel->GetTargetStackBytes();
				int delta = targetStackBytes - currentStackBytes;
				// doesn't make sense to jump to a place with more stack usage than the current point
				//assert(delta <= 0);
				amountOp->SetStackBytes(delta);
				break;
			}

			default:
			{
				VisitChildren(op);
				break;
			}
		}
	}

	void Visit(class IRLabelInstruction*) override {}
	void Visit(class IRReturnInstruction*) override {}
};
