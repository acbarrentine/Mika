#pragma once

#include "Identifier.h"
enum class MsgSeverity : int;

enum TType
{
	kOpenParen,
	kCloseParen,
	kOpenBrace,
	kCloseBrace,
	kComma,
	kColon,
	kDot,
	kPlus,
	kMinus,
	kSlash,
	kAsterisk,
	kArrow,
	kEquals,
	kLessEquals,
	kGreaterEquals,
	kLessThan,
	kGreaterThan,
	kNotEquals,

	kIf,
	kWhile,
	kAnd,
	kOr,
	kFun,
	kVoid,
	kInt,
	kFloat,
	kString,
	kStruct,
	kReturn,

	kIdentifier,
	kIntConstant,
	kFloatConstant,
	kStringConstant,

	kEOL,
	kEOF,
	kInvalid,
};

class Token
{
protected:
	TType mType;
	int mFileIndex;
	int mLineNumber;
	union
	{
		int mIntValue;
		double mFloatValue;
	};
	Identifier mStringValue;

public:
	Token(TType tokenType, int fileIndex, int lineNumber);

	void SetValue(int val) { mIntValue = val; }
	void SetValue(double val) { mFloatValue = val; }
	void SetValue(Identifier val) { mStringValue = val; }

	TType GetType() const { return mType; }
	int GetFileIndex() const { return mFileIndex; }
	int GetLineNumber() const { return mLineNumber; }
	Identifier GetIdentifier() const { return mStringValue; }

	void Print(MsgSeverity severity) const;

	static const char* StringRepresentation(TType tokenType);
};
