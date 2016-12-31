#pragma once

// IR is short for Intermediate Representation.
// That is a pretty generic term that could apply to the AST.
// By convention, though, IR refers to a pseudo-code
// instruction set, and is usually the last transformation
// before going to actual byte code.

#include "Identifier.h"

class Identifier;
class Variable;
class FunctionDeclaration;
class IRLabelInstruction;
class Label;

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs, numWrites) op,
enum OpCode : int
{
	IllegalInstruction,
#include "../MikaVM/MikaOpcodes.h"
};

#define VISITOR_CLASSES \
 	friend class DebugWriter; \
	friend class ReferenceCollector; \
	friend class VariableLocator; \
	friend class TempRegisterLocator; \
	friend class ByteCodeLocator; \
	friend class LabelLocator; \
	friend class ByteCodeWriter; \
	friend class Optimizer; \
	friend class StackPointerMover; \
	friend class IRVisitor;

class IRVisitor
{
public:
	virtual void Visit(class IRFunctionOperand*, bool forWrite) = 0;
	virtual void Visit(class IRVariableOperand*, bool forWrite) = 0;
	virtual void Visit(class IRRegisterOperand*, bool forWrite) = 0;
	virtual void Visit(class IRLabelOperand*, bool forWrite) = 0;
	virtual void Visit(class IRIntOperand*, bool forWrite) = 0;
	virtual void Visit(class IRFloatOperand*, bool forWrite) = 0;
	virtual void Visit(class IRStringOperand*, bool forWrite) = 0;
	virtual void Visit(class IRStackBytesOperand*, bool) = 0;

	virtual void Visit(class IRInstruction*) = 0;
	virtual void Visit(class IRLabelInstruction*) = 0;
	virtual void Visit(class IRReturnInstruction*) = 0;

	void VisitChildren(class IRInstruction* op);
};

class IRScope : public ManagedEntity
{
protected:
	IRScope* mParent;
	int mStackBytes;
	
	int GetParentStackBytes() const
	{
		return mParent ? mParent->GetStackBytes() : 0;
	}
	
public:
	IRScope(IRScope* parent)
		: mParent(parent)
		, mStackBytes(0) {}
	
	void SetStackOffset(int stackBytes)
	{
		int parentBytes = GetParentStackBytes();
		stackBytes -= parentBytes;
		if (stackBytes > mStackBytes)
			mStackBytes = stackBytes;
	}
	
	int GetStackBytes() const
	{
		return mStackBytes + GetParentStackBytes();
	}
};

class IROperand : public ManagedEntity
{
protected:
	int mReadCount;

public:
	IROperand() : mReadCount(0) {}
	virtual void Accept(IRVisitor* visitor, bool forWrite) = 0;

	void AddRead() { ++mReadCount; }
	int GetReadCount() const { return mReadCount; }
};

class IRFunctionOperand : public IROperand
{
protected:
	FunctionDeclaration* mFunction;
	int mStringOffset;

public:
	IRFunctionOperand(FunctionDeclaration* decl)
		: mFunction(decl)
		, mStringOffset(0)
	{
	}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRVariableOperand : public IROperand
{
protected:
	Variable* mVariable;
	
public:
	IRVariableOperand(Variable* var) : mVariable(var) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRRegisterOperand : public IROperand
{
protected:
	static int SDummyRegister;
	int mTempRegister;
	int mStackOffset;

public:
	IRRegisterOperand()
		: mTempRegister(SDummyRegister++)
		, mStackOffset(-1)
	{
	}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRLabelOperand : public IROperand
{
protected:
	Label* mLabel;
	int mByteCodeOffset;

public:
	IRLabelOperand(Label* label) : mLabel(label), mByteCodeOffset(0) {}
	void SetOffset(int byteCodeOffset) { mByteCodeOffset = byteCodeOffset; }
	int GetTargetStackBytes() const;

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRIntOperand : public IROperand
{
protected:
	int mValue;

public:
	IRIntOperand(int val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRFloatOperand : public IROperand
{
protected:
	double mValue;

public:
	IRFloatOperand(double val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;

};

class IRStringOperand : public IROperand
{
protected:
	Identifier mValue;
	int mStringOffset;

public:
	IRStringOperand(Identifier val)
		: mValue(val)
		, mStringOffset(0)
	{
	}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRStackBytesOperand : public IROperand
{
protected:
	int mNumBytes;
	bool mSubtract;

public:
	IRStackBytesOperand(bool subtract) : mNumBytes(0), mSubtract(subtract) {}

	void SetStackBytes(int numBytes) { mNumBytes = numBytes; }

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	VISITOR_CLASSES;
};

class IRInstruction : public ManagedEntity
{
public:
	struct OpCodeData
	{
		OpCode mCode;
		const char* mName;
		int mNumArgs;
		int mNumWrites;
	};

protected:
	OpCode mCode;
	int mRootToken;
	int mByteCodeOffset;
	bool mRemoved;
	IRScope* mScope;
	IROperand* mOperands[3];
	static OpCodeData SOpCodeData[];

public:
	IRInstruction(OpCode code, int rootToken, IRScope* scope)
		: mCode(code)
		, mRootToken(rootToken)
		, mByteCodeOffset(0)
		, mRemoved(false)
		, mScope(scope)
	{
		mOperands[0] = 0;
		mOperands[1] = 0;
		mOperands[2] = 0;
	}

	void SetOperand(int index, IROperand* op)
	{
		mOperands[index] = op;
		if (!WritesOperand(index))
		{
			op->AddRead();
		}
		else
		{
			assert(op->GetReadCount() == 0);
		}
	}

	virtual void SetByteCodeOffset(int byteCodeOffset)
	{
		mByteCodeOffset = byteCodeOffset;
	}

	virtual int GetSize() const;
	int GetNumOperands() const;
	const char* GetName() const;
	bool WritesOperand(int index);
	void Remove() { mRemoved = true; }
	IRScope* GetScope() const { return mScope; }
	
	virtual void Accept(IRVisitor* visitor)
	{
		if (!mRemoved) visitor->Visit(this);
	}

	VISITOR_CLASSES;
};

class IRLabelInstruction : public IRInstruction
{
protected:
	Label* mLabel;

public:
	IRLabelInstruction(Label* label, int rootToken, IRScope* scope)
		: IRInstruction(IllegalInstruction, rootToken, scope)
		, mLabel(label)
	{
	}

	virtual int GetSize() const override
	{
		return 0;
	}

	virtual void Accept(IRVisitor* visitor) override
	{
		if (!mRemoved) visitor->Visit(this);
	}

	virtual void SetByteCodeOffset(int byteCodeOffset) override;

	VISITOR_CLASSES;
};

class IRReturnInstruction : public IRInstruction
{
public:
	IRReturnInstruction(int rootToken, IRScope* scope)
		: IRInstruction(IllegalInstruction, rootToken, scope)
	{
	}

	virtual int GetSize() const override;

	virtual void Accept(IRVisitor* visitor) override
	{
		if (!mRemoved) visitor->Visit(this);
	}

	VISITOR_CLASSES;
};
