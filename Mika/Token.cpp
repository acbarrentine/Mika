#include "stdafx.h"
#include "Token.h"
#include "Compiler.h"


Token::Token(TType tokenType, int fileIndex, int lineNumber, const char* str, int len)
	: mType(tokenType)
	, mFileIndex(fileIndex)
	, mLineNumber(lineNumber)
	, mIntValue(0)
{
	mStringValue = GCompiler.AddIdentifier(str, str + len);
}

void Token::Print(MsgSeverity severity) const
{
	GCompiler.Message(severity, mStringValue.GetString());
}
