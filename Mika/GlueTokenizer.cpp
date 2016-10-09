#include "stdafx.h"
#include "Compiler.h"
#include "GlueTokenizer.h"


void GlueTokenizer::Read()
{
	int val;
	do
	{
		val = yylex();
	} while (val >= 0 && val != TType::kEOF);

	++mFileIndex;
}

void GlueTokenizer::LexError(const char c)
{
	const char* fileName = GCompiler.GetFileName(mFileIndex);
	GCompiler.Error("Illegal character '%c' found reading %s at line %d\n", c, fileName, yylineno);
}
