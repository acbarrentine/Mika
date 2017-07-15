#pragma once

#include "Identifier.h"
enum class MsgSeverity : int;

enum TType : int
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
	kPercent,
	kArrow,
	kEquals,
	kLessEquals,
	kGreaterEquals,
	kLessThan,
	kGreaterThan,
	kNotEquals,

	kIf,
	kElse,
	kWhile,
	kBreak,
	kContinue,
	kAnd,
	kOr,
	kFun,
	kVoid,
	kInt,
	kFloat,
	kString,
	kReturn,
	kLocation,
	kTypeName,

	kIdentifier,
	kIntConstant,
	kFloatConstant,
	kStringConstant,

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

	int GetIntValue() const { return mIntValue; }
	double GetFloatValue() const { return mFloatValue; }
	Identifier GetStringValue() const { return mStringValue; }

	TType GetType() const { return mType; }
	int GetFileIndex() const { return mFileIndex; }
	int GetLineNumber() const { return mLineNumber; }
	Identifier GetIdentifier() const { return mStringValue; }

	void Print(MsgSeverity severity) const;

	static const char* StringRepresentation(TType tokenType);
};
