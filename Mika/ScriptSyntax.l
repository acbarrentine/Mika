%top{
#include "stdafx.h"
}

%{
#include "Compiler.h"
#include "ScriptTokenizer.h"

#pragma warning( disable : 4244 )		// truncation
#define SYNTAX_TOKEN(type) GCompiler.CreateToken(type, mFileIndex, yylineno, yytext, yyleng); return type
%}

%option nodefault
%option yyclass="ScriptTokenizer"
%option noyywrap
%option c++
%option batch
%option prefix="Script"

DIGIT					[0-9]
LETTER					[a-zA-Z_]

%%

"("						{ SYNTAX_TOKEN(TType::kOpenParen); }
")"						{ SYNTAX_TOKEN(TType::kCloseParen); }
"{"						{ SYNTAX_TOKEN(TType::kOpenBrace); }
"}"						{ SYNTAX_TOKEN(TType::kCloseBrace); }
","						{ SYNTAX_TOKEN(TType::kComma); }
":"						{ SYNTAX_TOKEN(TType::kColon); }
"."						{ SYNTAX_TOKEN(TType::kDot); }
"+"						{ SYNTAX_TOKEN(TType::kPlus); }
"-"						{ SYNTAX_TOKEN(TType::kMinus); }
"/"						{ SYNTAX_TOKEN(TType::kSlash); }
"*"						{ SYNTAX_TOKEN(TType::kAsterisk); }
"<-"					{ SYNTAX_TOKEN(TType::kArrow); }
"=="					{ SYNTAX_TOKEN(TType::kEquals); }
"<="					{ SYNTAX_TOKEN(TType::kLessEquals); }
">="					{ SYNTAX_TOKEN(TType::kGreaterEquals); }
"<"						{ SYNTAX_TOKEN(TType::kLessThan); }
">"						{ SYNTAX_TOKEN(TType::kGreaterThan); }
"!="					{ SYNTAX_TOKEN(TType::kNotEquals); }

"if"					{ SYNTAX_TOKEN(TType::kIf); }
"while"					{ SYNTAX_TOKEN(TType::kWhile); }
"and"					{ SYNTAX_TOKEN(TType::kAnd); }
"or"					{ SYNTAX_TOKEN(TType::kOr); }
"fun"					{ SYNTAX_TOKEN(TType::kFun); }
"void"					{ SYNTAX_TOKEN(TType::kVoid); }
"int"					{ SYNTAX_TOKEN(TType::kInt); }
"float"					{ SYNTAX_TOKEN(TType::kFloat); }
"string"				{ SYNTAX_TOKEN(TType::kString); }
"struct"				{ SYNTAX_TOKEN(TType::kStruct); }
"return"				{ SYNTAX_TOKEN(TType::kReturn); }

	/* Identifier */
{LETTER}({LETTER}|{DIGIT})*					{
												GCompiler.CreateToken(TType::kIdentifier, mFileIndex, yylineno, yytext, yyleng);
												return TType::kIdentifier;
											}

	/* Decimal constant */
{DIGIT}+									{
												Token& t = GCompiler.CreateToken(TType::kIntConstant, mFileIndex, yylineno, yytext, yyleng);
												t.SetValue(1);
												return TType::kInt;
											}

	/* Float constant */
{DIGIT}+"."{DIGIT}+							{}

	/* String constant */
\".*\"										{}

	/* Whitespace */
[ \r\v\f]				{}
[\t]					{}
[\n]					{	
							GCompiler.CreateToken(TType::kEOL, mFileIndex, yylineno++, yytext, yyleng);
							return TType::kEOL;
						}
<<EOF>>					{ SYNTAX_TOKEN(TType::kEOF); }

	/* Error check */
.						{ LexError( *yytext ); }

%%
