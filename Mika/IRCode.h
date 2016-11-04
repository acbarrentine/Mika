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
	FunctionDeclaration* mFunction;
public:
	IRFunctionOperand(FunctionDeclaration* decl) : mFunction(decl) {}
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
	virtual void DebugSerialize(std::ostream& stream)
	{
		stream << "%" << mTempRegister;
	}
};

class IRLabelOperand : public IROperand
{
	int mLabel;
};

class IRIntOperand : public IROperand
{
	int mValue;
public:
	IRIntOperand(int val) : mValue(val) {}
	virtual void DebugSerialize(std::ostream& stream)
	{
		stream << mValue;
	}
};

class IRFloatOperand : public IROperand
{
	double mValue;
public:
	IRFloatOperand(double val) : mValue(val) {}
};

class IRStringOperand : public IROperand
{
	Identifier mValue;
public:
	IRStringOperand(Identifier val) : mValue(val) {}
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
