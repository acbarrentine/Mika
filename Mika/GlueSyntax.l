%top{
#include "stdafx.h"
}

%{
#include "Compiler.h"
#include "GlueTokenizer.h"

#pragma warning( disable : 4244 )		// truncation
#define SYNTAX_TOKEN(type) GCompiler.CreateToken(type, mFileIndex, yylineno); return type
%}

%option nodefault
%option yyclass="GlueTokenizer"
%option noyywrap
%option c++
%option batch
%option prefix="Glue"

DIGIT					[0-9]
LETTER					[a-zA-Z_]

%%

"("						{ SYNTAX_TOKEN(TType::kOpenParen); }
")"						{ SYNTAX_TOKEN(TType::kCloseParen); }
"{"						{ SYNTAX_TOKEN(TType::kOpenBrace); }
"}"						{ SYNTAX_TOKEN(TType::kCloseBrace); }
","						{ SYNTAX_TOKEN(TType::kComma); }
":"						{ SYNTAX_TOKEN(TType::kColon); }

"void"					{ SYNTAX_TOKEN(TType::kVoid); }
"int"					{ SYNTAX_TOKEN(TType::kInt); }
"float"					{ SYNTAX_TOKEN(TType::kFloat); }
"string"				{ SYNTAX_TOKEN(TType::kString); }
"struct"				{ SYNTAX_TOKEN(TType::kStruct); }

	/* Identifier */
{LETTER}({LETTER}|{DIGIT})*					{
												Token& t = GCompiler.CreateToken(TType::kIdentifier, mFileIndex, yylineno);
												t.SetValue(GCompiler.AddIdentifier(yytext, yytext + yyleng));
												return TType::kIdentifier;
											}

	/* Whitespace */
[ \r\v\f]				{}
[\t]					{}
[\n]					{	
							GCompiler.CreateToken(TType::kEOL, mFileIndex, yylineno++);
							return TType::kEOL;
						}
<<EOF>>					{ SYNTAX_TOKEN(TType::kEOF); }

	/* Error check */
.						{ LexError( *yytext ); }

%%
