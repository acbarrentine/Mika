%top{
#include "stdafx.h"
}

%{
#include "Compiler.h"
#include "ScriptTokenizer.h"

#pragma warning(disable : 4244)			// truncation
#pragma warning(disable : 4267)			// truncation
#pragma warning(disable : 6011)			// analysis - write to null ptr

#define SYNTAX_TOKEN(type) GCompiler.CreateToken(type, mFileIndex, yylineno); return type
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
"else"					{ SYNTAX_TOKEN(TType::kElse); }
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
												Token& t = GCompiler.CreateToken(TType::kIdentifier, mFileIndex, yylineno);
												t.SetValue(GCompiler.AddIdentifier(yytext, yytext + yyleng));
												return TType::kIdentifier;
											}

	/* Decimal constant */
{DIGIT}+									{
												Token& t = GCompiler.CreateToken(TType::kIntConstant, mFileIndex, yylineno);
												t.SetValue(ParseInt());
												t.SetValue(GCompiler.AddIdentifier(yytext, yytext + yyleng));
												return TType::kInt;
											}

	/* Float constant */
{DIGIT}+"."{DIGIT}+							{
												Token& t = GCompiler.CreateToken(TType::kFloatConstant, mFileIndex, yylineno);
												t.SetValue(ParseDouble());
												t.SetValue(GCompiler.AddIdentifier(yytext, yytext + yyleng));
												return TType::kFloat;
											}

	/* String constant */
\".*\"										{
												char stringBuf[2048];
												strncpy_s(stringBuf, ARRAY_COUNT(stringBuf), yytext + 1, yyleng - 2);
												Token& t = GCompiler.CreateToken(TType::kStringConstant, mFileIndex, yylineno);
												t.SetValue(GCompiler.AddIdentifier(stringBuf, stringBuf + (yyleng - 2)));
												return TType::kStringConstant;
											}

	/* Whitespace */
[ \r\v\f]				{}
[\t]					{}
[\n]					{ ++yylineno; }
[;]						{}
<<EOF>>					{ SYNTAX_TOKEN(TType::kEOF); }

	/* Comment */
#.*$					{}

	/* Error check */
.						{ LexError( *yytext ); }

%%
