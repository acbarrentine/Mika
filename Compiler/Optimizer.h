#pragma once

// A very basic peephole optimizer.
// Compare pairs of instructions and see if you can make them better.
class Optimizer : public IRVisitor
{
protected:
	IRInstruction* mPrev;

public:
	Optimizer()
		: mPrev(nullptr)
	{
	}

	void Visit(IRVariableOperand*, bool) override {}
	void Visit(IRFunctionOperand*, bool) override {}
	void Visit(IRStringOperand*, bool) override {}
	void Visit(IRRegisterOperand*, bool) override {}
	void Visit(IRLabelOperand*, bool) override {}
	void Visit(IRIntOperand*, bool) override {}
	void Visit(IRFloatOperand*, bool) override {}
	void Visit(IRStackBytesOperand*, bool) override {}

	void Visit(IRInstruction* op)
	{
		// look for the use of a temporary holding a value about to
		// be moved to a variable
		if (mPrev && op->mCode == CopyStackToStack)
		{
			switch (mPrev->mCode)
			{
				case AddInt:
				case AddFloat:
				case SubtractInt:
				case SubtractFloat:
				case DivideInt:
				case DivideFloat:
				case MultiplyInt:
				case MultiplyFloat:
				case EqualsInt:
				case EqualsFloat:
				case EqualsString:
				case LessEqualsInt:
				case LessEqualsFloat:
				case LessEqualsString:
				case GreaterEqualsInt:
				case GreaterEqualsFloat:
				case GreaterEqualsString:
				case LessThanInt:
				case LessThanFloat:
				case LessThanString:
				case GreaterThanInt:
				case GreaterThanFloat:
				case GreaterThanString:
				case NotEqualsInt:
				case NotEqualsFloat:
				case NotEqualsString:
					if (mPrev->mOperands[0] == op->mOperands[1])
					{
						op->Remove();
						mPrev->mOperands[0] = op->mOperands[0];
					}
					break;

				default:
					break;
			}
		}
		mPrev = op;
	}

	void Visit(IRLabelInstruction* op) override
	{
		if (mPrev && mPrev->mCode == UnconditionalBranch)
		{
			IRLabelOperand* target = (IRLabelOperand*)mPrev->mOperands[0];
			if (target->mLabel == op->mLabel)
			{
				mPrev->Remove();
			}
		}
		mPrev = op;
	}

	void Visit(IRReturnInstruction* op) override
	{
		mPrev = op;
	}

	void OptimizeFunction(ObjectFileHelper::FunctionRecord& record)
	{
		for (IRInstruction* op : record.mInstructions)
		{
			op->Accept(this);
		}
	}
};
