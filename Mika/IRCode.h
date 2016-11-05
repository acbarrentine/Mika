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
#define MIKA_OPCODE(op, numArgs) op,
enum OpCode : int
{
	IllegalInstruction,
#include "..\MikaVM\MikaOpcodes.h"
};

class IRVisitor
{
public:
	virtual void Visit(class IROperand*) = 0;
	virtual void Visit(class IRFunctionOperand*) = 0;
	virtual void Visit(class IRVariableOperand*) = 0;
	virtual void Visit(class IRRegisterOperand*) = 0;
	virtual void Visit(class IRLabelOperand*) = 0;
	virtual void Visit(class IRIntOperand*) = 0;
	virtual void Visit(class IRFloatOperand*) = 0;
	virtual void Visit(class IRStringOperand*) = 0;
	virtual void Visit(class IRInstruction*) = 0;
};

class IROperand
{
public:
	virtual void Accept(IRVisitor* visitor) = 0;
};

class IRFunctionOperand : public IROperand
{
protected:
	FunctionDeclaration* mFunction;
public:
	IRFunctionOperand(FunctionDeclaration* decl) : mFunction(decl) {}

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRVariableOperand : public IROperand
{
protected:
	Variable* mVariable;
public:
	IRVariableOperand(Variable* var) : mVariable(var) {}

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRRegisterOperand : public IROperand
{
protected:
	static int SDummyRegister;
	int mTempRegister;
public:
	IRRegisterOperand() : mTempRegister(SDummyRegister++) {}

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRLabelOperand : public IROperand
{
protected:
	int mByteCodeOffset;
public:
	IRLabelOperand() : mByteCodeOffset(0) {}
	void SetOffset(int byteCodeOffset) { mByteCodeOffset = byteCodeOffset; }

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRIntOperand : public IROperand
{
protected:
	int mValue;
public:
	IRIntOperand(int val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRFloatOperand : public IROperand
{
protected:
	double mValue;
public:
	IRFloatOperand(double val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRStringOperand : public IROperand
{
protected:
	Identifier mValue;

public:
	IRStringOperand(Identifier val) : mValue(val) {}

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};

class IRInstruction
{
public:
	struct OpCodeData
	{
		OpCode mCode;
		const char* mName;
		int mNumArgs;
	};

protected:
	OpCode mCode;
	size_t mRootToken;
	int mByteCodeOffset;
	IROperand* mOperands[3];
	static OpCodeData SOpCodeData[];

public:
	IRInstruction(OpCode code, size_t rootToken, int byteCodeOffset)
		: mCode(code)
		, mRootToken(rootToken)
		, mByteCodeOffset(byteCodeOffset)
	{
		mOperands[0] = 0;
		mOperands[1] = 0;
		mOperands[2] = 0;
	}

	void SetOperand(int index, IROperand* op)
	{
		mOperands[index] = op;
	}

	int GetSize() const;

	virtual void Accept(IRVisitor* visitor) { visitor->Visit(this); }
	friend class DebugWriter;
};
