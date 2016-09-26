%top{
#include "stdafx.h"

}

%{
#include "Tokenizer.h"
#pragma warning( disable : 4244 )		// truncation

%}

%option debug
%option nodefault
%option yyclass="Tokenizer"
%option noyywrap
%option c++

%%
%%
