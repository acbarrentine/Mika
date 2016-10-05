#pragma once

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer ScriptFlexLexer
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int ScriptTokenizer::yylex()

class ScriptTokenizer : public yyFlexLexer
{
protected:
	int mFileIndex;

public:
	ScriptTokenizer(std::istream* in)
		: yyFlexLexer(in)
		, mFileIndex(0)
	{
	}

	int yylex();

	void Read();
	void LexError(const char c);

	int ParseInt();
	double ParseDouble();
};
