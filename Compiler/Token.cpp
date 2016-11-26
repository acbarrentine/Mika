#include "stdafx.h"
#include "Token.h"
#include "Compiler.h"


Token::Token(TType tokenType, int fileIndex, int lineNumber)
	: mType(tokenType)
	, mFileIndex(fileIndex)
	, mLineNumber(lineNumber)
	, mIntValue(0)
{
}

void Token::Print(MsgSeverity severity) const
{
	const char* stringRep = StringRepresentation(mType);
	if (!stringRep)
	{
		if (mType == TType::kStringConstant)
		{
			GCompiler.Message(severity, "\"%s\"", mStringValue.GetString());
		}
		else
		{
			GCompiler.Message(severity, mStringValue.GetString());
		}
	}
	else
	{
		GCompiler.Message(severity, stringRep);
	}
}

const char* Token::StringRepresentation(TType tokenType)
{
	switch (tokenType)
	{
		case kOpenParen:		return "(";
		case kCloseParen:		return ")";
		case kOpenBrace:		return "{";
		case kCloseBrace:		return "}";
		case kComma:			return ",";
		case kColon:			return ":";
		case kDot:				return ".";
		case kPlus:				return "+";
		case kMinus:			return "-";
		case kSlash:			return "/";
		case kAsterisk:			return "*";
		case kArrow:			return "<-";
		case kEquals:			return "==";
		case kLessEquals:		return "<=";
		case kGreaterEquals:	return ">=";
		case kLessThan:			return "<";
		case kGreaterThan:		return ">";
		case kNotEquals:		return "!=";

		case kIf:				return "if";
		case kElse:				return "else";
		case kWhile:			return "while";
		case kBreak:			return "break";
		case kContinue:			return "continue";
		case kAnd:				return "and";
		case kOr:				return "or";
		case kFun:				return "fun";
		case kVoid:				return "void";
		case kInt:				return "int";
		case kFloat:			return "float";
		case kString:			return "string";
		case kStruct:			return "struct";
		case kReturn:			return "return";
		case kLocation:			return "location";

		case kEOF:				return "<<EOF>>";

		default:				return nullptr;
	}
}
