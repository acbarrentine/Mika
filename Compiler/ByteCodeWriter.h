#pragma once

#include "IRCode.h"

class ByteCodeWriter : public IRVisitor
{
protected:
	struct ByteStream : public MikaArchive
	{
		std::vector<char>& mByteCode;
		char* mHead;

		ByteStream(std::vector<char>& byteCode)
			: mByteCode(byteCode)
		{
		}

		virtual void Serialize(void* v, int size) override
		{
			size_t start = mByteCode.size();
			mByteCode.insert(mByteCode.end(), size, 0);
			memcpy(&mByteCode[start], v, size);
		}

		int Tell() const
		{
			return (int)mByteCode.size();
		}
	} mByteStream;

	std::vector<IRInstruction*>& mInstructions;

public:
	ByteCodeWriter(ObjectFileHelper::FunctionRecord& record)
		: mByteStream(record.mByteCodeData)
		, mInstructions(record.mInstructions)
	{
	}

	void Visit(IRFunctionOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mIntVal = op->mStringOffset;
		mByteStream << cell;
	}

	void Visit(IRVariableOperand* op, bool) override
	{
		MikaArchiveCell cell;
		Variable* var = op->mVariable;
		cell.mStackIndex.mOffset = var->GetStackOffset();
		cell.mStackIndex.mGlobal = var->GetIsGlobal() ? 1 : 0;
		mByteStream << cell;
	}

	void Visit(IRRegisterOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mStackIndex.mOffset = op->mStackOffset;
		cell.mStackIndex.mGlobal = 0;
		mByteStream << cell;
	}

	void Visit(IRLabelOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mIntVal = op->mByteCodeOffset;
		mByteStream << cell;
	}

	void Visit(IRIntOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mIntVal = op->mValue;
		mByteStream << cell;
	}

	void Visit(IRFloatOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mDblVal = op->mValue;
		mByteStream << cell;
	}

	void Visit(IRStringOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mIntVal = op->mStringOffset;
		mByteStream << cell;
	}

	void Visit(IRStackBytesOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mIntVal = op->mNumBytes;
		mByteStream << cell;
	}

	static_assert(sizeof(MikaArchiveInstruction) == sizeof(MikaVM::Instruction), "Compiler and runtime disagreement on instruction size");
	static_assert(sizeof(MikaArchiveCell) == sizeof(MikaVM::Cell), "Compiler and runtime disagree on Cell size");

	void Visit(IRInstruction* op) override
	{
		// double check that we've written the right amount of data
		assert(mByteStream.Tell() == op->mByteCodeOffset);

		MikaArchiveInstruction instuction;
		instuction.mPlaceholder = nullptr;
		instuction.mCode = op->mCode;
		
		Token& tok = GCompiler.GetToken(op->mRootToken);
		instuction.mLineNumber = (short)tok.GetLineNumber();
		instuction.mNumArgs = (short)op->GetNumOperands();
		instuction.mFlags = 0;
		
		mByteStream << instuction;

		VisitChildren(op);
	}

	void Visit(IRLabelInstruction*) override
	{
	}
	
	void Visit(IRReturnInstruction*) override
	{
		MikaArchiveCell term;
		term.mIntVal = 0;
		mByteStream << term;
	}

	void WriteFunction()
	{
		for (IRInstruction* op : mInstructions)
		{
			op->Accept(this);
		}
	}
};
