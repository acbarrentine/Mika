#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int Tokenizer::yylex()

enum TType
{
	kOpenParen,
	kCloseParen,
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

	kEOF
};

class Token
{
protected:
	TType mType;
	int mLineNumber;
	union
	{
		int mIntValue;
		float mFloatValue;
		const char* mStringValue;
	};

public:
	Token(TType tokenType, int lineNumber)
		: mType(tokenType)
		, mLineNumber(lineNumber)
		, mStringValue(nullptr)
	{}
};

class Tokenizer : public yyFlexLexer
{
protected:
	const char* mFileName;

public:
	Tokenizer(std::istream* inputStream, std::ostream* outputStream)
		: yyFlexLexer(inputStream, outputStream)
		, mFileName(nullptr)
	{}

	int yylex();

	void Read(const char* fileName);
	void LexError(const char c);
};
