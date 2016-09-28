#include "stdafx.h"
#include "Compiler.h"

void Tokenizer::Read(const char* fileName)
{
	mFileName = fileName;

	int val;
	do
	{
		val = yylex();
	} while (val >= 0 && val != TType::kEOF);
}

void Tokenizer::LexError(const char c)
{
	GCompiler.Message(MsgSeverity::kError, "Illegal character ('%c') found reading %s at line %d\n", c, mFileName, yylineno);
}
