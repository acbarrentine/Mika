#include "stdafx.h"
#include "BinaryExpression.h"
#include "Compiler.h"

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

void BinaryExpression::ResolveType()
{

}
