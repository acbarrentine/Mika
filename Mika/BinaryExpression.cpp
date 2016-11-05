#include "stdafx.h"
#include "BinaryExpression.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"


int BinaryExpression::GetPrecedence() const
{
	Token& t = GCompiler.GetToken(mRootToken);
	TType tokenType = t.GetType();
	switch (tokenType)
	{
		case TType::kPlus:
			return 14;
		case TType::kMinus:
			return 14;
		case TType::kSlash:
			return 15;
		case TType::kAsterisk:
			return 15;
		case TType::kArrow:
			return 4;
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

	mType = leftType;
}

void BinaryExpression::GenCode(ObjectFileHelper& helper)
{
	mLeft->GenCode(helper);
	mRight->GenCode(helper);

	mResultRegister = new IRRegisterOperand();
}
