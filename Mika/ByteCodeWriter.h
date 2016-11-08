#pragma once

#include "IRCode.h"

class ByteCodeWriter : public IRVisitor
{
protected:
	struct ByteStream : public MikaArchive
	{
		std::vector<unsigned char>& mByteCode;
		unsigned char* mHead;

		ByteStream(std::vector<unsigned char>& byteCode)
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
		cell.mIntVal = op->mVariable->GetStackOffset();
		mByteStream << cell;
	}

	void Visit(IRRegisterOperand* op, bool) override
	{
		MikaArchiveCell cell;
		cell.mIntVal = op->mStackOffset;
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
		instuction.mFileIndex = (short)tok.GetFileIndex();
		instuction.mLineNumber = (short)tok.GetLineNumber();
		instuction.mFlags = 0;
		instuction.mNumArgs = (short)op->GetNumOperands();
		
		mByteStream << instuction;

		int numOperands = op->GetNumOperands();
		for (int i = 0; i < numOperands; ++i)
		{
			IROperand* operand = op->mOperands[i];
			operand->Accept(this, op->WritesOperand(i));
		}
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
