#include "stdafx.h"
#include "Compiler.h"

Token::Token(TType tokenType, int fileIndex, int lineNumber, const char* str, int len)
	: mType(tokenType)
	, mFileIndex(fileIndex)
	, mLineNumber(lineNumber)
	, mIntValue(0)
{
	mStringValue = GCompiler.AddIdentifier(str, str + len);
}

void Token::Print() const
{
	GCompiler.Message(MsgSeverity::kInfo, mStringValue.GetString());
}

void Tokenizer::Read()
{
	int val;
	do
	{
		val = yylex();
	} while (val >= 0 && val != TType::kEOF);

	++mFileIndex;
}

void Tokenizer::LexError(const char c)
{
	const char* fileName = GCompiler.GetFileName(mFileIndex);
	GCompiler.Message(MsgSeverity::kError, "Illegal character '%c' found reading %s at line %d\n", c, fileName, yylineno);
}
