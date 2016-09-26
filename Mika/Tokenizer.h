#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef  YY_DECL
#define YY_DECL int Tokenizer::yylex()

class Tokenizer : public yyFlexLexer
{
public:
	Tokenizer(/*char* buffer, size_t len*/)
		: yyFlexLexer(nullptr)
	{
	}

	int yylex();
};
