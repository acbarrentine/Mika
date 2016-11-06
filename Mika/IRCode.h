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

#undef MIKA_OPCODE
#define MIKA_OPCODE(op, numArgs, numWrites) op,
enum OpCode : int
{
	IllegalInstruction,
#include "..\MikaVM\MikaOpcodes.h"
};

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
	
	virtual void Visit(class IRInstruction*) = 0;
	virtual void Visit(class IRLabel*) = 0;
	virtual void Visit(class IRReturnInstruction*) = 0;
};

class IROperand
{
public:
	virtual void Accept(IRVisitor* visitor, bool forWrite) = 0;
};

class IRFunctionOperand : public IROperand
{
protected:
	FunctionDeclaration* mFunction;

public:
	IRFunctionOperand(FunctionDeclaration* decl) : mFunction(decl) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRVariableOperand : public IROperand
{
protected:
	Variable* mVariable;
public:
	IRVariableOperand(Variable* var) : mVariable(var) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
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
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRLabelOperand : public IROperand
{
protected:
	int mByteCodeOffset;

public:
	IRLabelOperand() : mByteCodeOffset(0) {}
	void SetOffset(int byteCodeOffset) { mByteCodeOffset = byteCodeOffset; }

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRIntOperand : public IROperand
{
protected:
	int mValue;

public:
	IRIntOperand(int val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRFloatOperand : public IROperand
{
protected:
	double mValue;

public:
	IRFloatOperand(double val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRStringOperand : public IROperand
{
protected:
	Identifier mValue;

public:
	IRStringOperand(Identifier val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor, bool forWrite) { visitor->Visit(this, forWrite); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRInstruction
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
	IROperand* mOperands[3];
	static OpCodeData SOpCodeData[];

public:
	IRInstruction(OpCode code, int rootToken)
		: mCode(code)
		, mRootToken(rootToken)
		, mByteCodeOffset(0)
	{
		mOperands[0] = 0;
		mOperands[1] = 0;
		mOperands[2] = 0;
	}

	void SetOperand(int index, IROperand* op)
	{
		mOperands[index] = op;
	}

	virtual int GetSize() const;
	int GetNumOperands() const;
	const char* GetName() const;
	bool WritesOperand(int index);

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRLabel : public IRInstruction
{
protected:
	IRLabelOperand* mLabel;

public:
	IRLabel(IRLabelOperand* label, int rootToken)
		: IRInstruction(IllegalInstruction, rootToken)
		, mLabel(label)
	{
	}

	virtual int GetSize() const override
	{
		return 0;
	}

	virtual void Accept(IRVisitor* visitor) override { visitor->Visit(this); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};

class IRReturnInstruction : public IRInstruction
{
public:
	IRReturnInstruction(int rootToken)
		: IRInstruction(IllegalInstruction, rootToken)
	{
	}

	virtual int GetSize() const override;

	virtual void Accept(IRVisitor* visitor) override { visitor->Visit(this); }
	friend class DebugWriter;
	friend class ReferenceCollector;
	friend class VariableLocator;
	friend class TempRegisterLocator;
};
