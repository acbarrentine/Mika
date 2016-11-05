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
enum OpCode
{
	IllegalInstruction,
#include "..\MikaVM\MikaOpcodes.h"
};

class IROperand
{
public:
	virtual void DebugSerialize(std::ostream&) {}
};

class IRFunctionOperand : public IROperand
{
protected:
	FunctionDeclaration* mFunction;
public:
	IRFunctionOperand(FunctionDeclaration* decl) : mFunction(decl) {}
	virtual void DebugSerialize(std::ostream& stream) override;
};

class IRVariableOperand : public IROperand
{
	Variable* mVariable;
public:
	IRVariableOperand(Variable* var) : mVariable(var) {}
};

class IRRegisterOperand : public IROperand
{
	static int SDummyRegister;
	int mTempRegister;
public:
	IRRegisterOperand() : mTempRegister(SDummyRegister++) {}
	virtual void DebugSerialize(std::ostream& stream) override;
};

class IRLabelOperand : public IROperand
{
protected:
	int mByteCodeOffset;
public:
	IRLabelOperand() : mByteCodeOffset(0) {}
	void SetOffset(int byteCodeOffset) { mByteCodeOffset = byteCodeOffset; }
	virtual void DebugSerialize(std::ostream& stream) override;
};

class IRIntOperand : public IROperand
{
	int mValue;
public:
	IRIntOperand(int val) : mValue(val) {}
	virtual void DebugSerialize(std::ostream& stream) override;
};

class IRFloatOperand : public IROperand
{
	double mValue;
public:
	IRFloatOperand(double val) : mValue(val) {}
	virtual void DebugSerialize(std::ostream& stream) override;
};

class IRStringOperand : public IROperand
{
	Identifier mValue;
public:
	IRStringOperand(Identifier val) : mValue(val) {}
	virtual void DebugSerialize(std::ostream& stream) override;
};

class IRInstruction
{
protected:
	OpCode mCode;
	size_t mRootToken;
	int mByteCodeOffset;
	IROperand* mOperands[3];

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

	friend std::ostream& operator<<(std::ostream& stream, IRInstruction& op);
};
