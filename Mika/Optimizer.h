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

	void Visit(IRInstruction* op)
	{
		mPrev = op;
	}

	void Visit(IRLabelInstruction* op) override
	{
		mPrev = op;
	}

	void Visit(IRReturnInstruction* op) override
	{
		// a manual return statement at the end of a function will generate
		// two of these in a row
		IRReturnInstruction* previousReturn = dynamic_cast<IRReturnInstruction*>(mPrev);
		if (previousReturn)
		{
			op->Remove();
		}
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
