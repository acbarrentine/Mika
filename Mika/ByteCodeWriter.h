#pragma once

#include "IRCode.h"

class ByteCodeWriter : public IRVisitor
{
protected:
	std::vector<unsigned char>& mByteCode;

public:
	ByteCodeWriter(std::vector<unsigned char>& byteCode)
		: mByteCode(byteCode)
	{
	}

	void Visit(IRFunctionOperand* op, bool) override
	{
		op;
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		op;
	}

	void Visit(IRRegisterOperand* op, bool) override
	{
		op;
	}

	void Visit(IRLabelOperand* op, bool) override
	{
		op;
	}

	void Visit(IRIntOperand* op, bool) override
	{
		op;
	}

	void Visit(IRFloatOperand* op, bool) override
	{
		op;
	}

	void Visit(IRStringOperand* op, bool) override
	{
		op;
	}

	void Visit(IRInstruction* op) override
	{
		op;
	}

	void Visit(class IRLabel*) override
	{
	}
	
	void Visit(class IRReturnInstruction*) override
	{
	}


	void WriteFunction(ObjectFileHelper::FunctionRecord& record)
	{
		for (IRInstruction* op : record.mInstructions)
		{
			Visit(op);
		}
	}
};
