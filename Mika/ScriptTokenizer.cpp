#include "stdafx.h"
#include "Compiler.h"
#include "ScriptTokenizer.h"

void ScriptTokenizer::Read()
{
	int val;
	do
	{
		val = yylex();
	} while (val >= 0 && val != TType::kEOF);

	++mFileIndex;
}

void ScriptTokenizer::LexError(const char c)
{
	const char* fileName = GCompiler.GetFileName(mFileIndex);
	GCompiler.Message(MsgSeverity::kError, "Illegal character '%c' found reading %s at line %d\n", c, fileName, yylineno);
}
