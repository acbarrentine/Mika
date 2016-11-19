#pragma once

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer GlueFlexLexer
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int GlueTokenizer::yylex()

class GlueTokenizer : public yyFlexLexer
{
protected:
	int mFileIndex;

public:
	GlueTokenizer(std::istream* in, int fileIndex)
		: yyFlexLexer(in)
		, mFileIndex(fileIndex)
	{
	}

	int yylex();

	void Read();
	void LexError(const char c);
};
