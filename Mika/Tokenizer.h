#pragma once

#include "Identifier.h"

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int Tokenizer::yylex()

enum TType
{
	kOpenParen,
	kCloseParen,
	kOpenBrace,
	kCloseBrace,
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
	kEOF
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
		float mFloatValue;
	};
	Identifier mStringValue;

public:
	Token(TType tokenType, int fileIndex, int lineNumber, const char* str, int len);

	void SetValue(int val) { mIntValue = val; }
	void SetValue(float val) { mFloatValue = val; }
	void SetValue(Identifier val) { mStringValue = val; }

	void Print() const;
};

class Tokenizer : public yyFlexLexer
{
protected:
	int mFileIndex;

public:
	Tokenizer(std::istream* in)
		: yyFlexLexer(in)
		, mFileIndex(0)
	{
	}

	int yylex();

	void Read();
	void LexError(const char c);
};
