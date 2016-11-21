#include "stdafx.h"
#include "BinaryExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"

BinaryExpression::BinaryExpression(int rootToken)
	: Expression(rootToken)
	, mLeft(nullptr)
	, mRight(nullptr)
	, mOpCode(IllegalInstruction)
{
	Token& t = GCompiler.GetToken(rootToken);
	mTokenType = t.GetType();
}

int BinaryExpression::GetPrecedence() const
{
	switch (mTokenType)
	{
		case TType::kArrow:
			return 4;
		case TType::kPlus:
			return 14;
		case TType::kMinus:
			return 14;
		case TType::kSlash:
			return 15;
		case TType::kAsterisk:
			return 15;
		case TType::kEquals:
			return 11;
		case TType::kLessEquals:
			return 12;
		case TType::kGreaterEquals:
			return 12;
		case TType::kLessThan:
			return 12;
		case TType::kGreaterThan:
			return 12;
		case TType::kNotEquals:
			return 11;

		default:
			GCompiler.Error(mRootToken, "invalid token type for binary operator");
			return 0;
	}
}

void BinaryExpression::ResolveType(SymbolTable& symbolTable)
{
	mLeft->ResolveType(symbolTable);
	mRight->ResolveType(symbolTable);

	Type* leftType = mLeft->GetType();
	Type* rightType = mRight->GetType();
	if (leftType != rightType)
	{
		GCompiler.Error(mRootToken, "incompatible types in expression");
	}

	Type* intType = GCompiler.FindType(TType::kInt);
	Type* floatType = GCompiler.FindType(TType::kFloat);

	int opType;
	if (leftType == intType)
		opType = 0;
	else if (leftType == floatType)
		opType = 1;
	else
		opType = 2;

	const static OpCode AddOps[] = { AddInt, AddFloat, IllegalInstruction };
	const static OpCode SubtractOps[] = { SubtractInt, SubtractFloat, IllegalInstruction };
	const static OpCode DivideOps[] = { DivideInt, DivideFloat, IllegalInstruction };
	const static OpCode MultiplyOps[] = { MultiplyInt, MultiplyFloat, IllegalInstruction };
	const static OpCode EqualsOps[] = { EqualsInt, EqualsFloat, EqualsString };
	const static OpCode LEOps[] = { LessEqualsInt, LessEqualsFloat, LessEqualsString };
	const static OpCode GEOps[] = { GreaterEqualsInt, GreaterEqualsFloat, GreaterEqualsString };
	const static OpCode LTOps[] = { LessThanInt, LessThanFloat, LessThanString };
	const static OpCode GTOps[] = { GreaterThanInt, GreaterThanFloat, GreaterThanString };
	const static OpCode NEOps[] = { NotEqualsInt, NotEqualsFloat, NotEqualsString };

	switch (mTokenType)
	{
		case TType::kPlus:
			mOpCode = AddOps[opType];
			mType = leftType;
			break;
		case TType::kMinus:
			mOpCode = SubtractOps[opType];
			mType = leftType;
			break;
		case TType::kSlash:
			mOpCode = DivideOps[opType];
			mType = leftType;
			break;
		case TType::kAsterisk:
			mOpCode = MultiplyOps[opType];
			mType = leftType;
			break;

		case TType::kEquals:
			mOpCode = EqualsOps[opType];
			mType = intType;
			break;
		case TType::kLessEquals:
			mOpCode = LEOps[opType];
			mType = intType;
			break;
		case TType::kGreaterEquals:
			mOpCode = GEOps[opType];
			mType = intType;
			break;
		case TType::kLessThan:
			mOpCode = LTOps[opType];
			mType = intType;
			break;
		case TType::kGreaterThan:
			mOpCode = GTOps[opType];
			mType = intType;
			break;
		case TType::kNotEquals:
			mOpCode = NEOps[opType];
			mType = intType;
			break;
	}

	if (mOpCode == IllegalInstruction)
	{
		GCompiler.Error(mRootToken, "illegal type for binary operator");
	}
}

void BinaryExpression::GenCode(ObjectFileHelper& helper)
{
	mLeft->GenCode(helper);
	mRight->GenCode(helper);

	mResultRegister = new IRRegisterOperand();
	IRInstruction* binOp = helper.EmitInstruction(mOpCode, mRootToken);
	binOp->SetOperand(0, mResultRegister);
	binOp->SetOperand(1, mLeft->GetResultRegister());
	binOp->SetOperand(2, mRight->GetResultRegister());
}
