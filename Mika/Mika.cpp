// Mika.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Compiler.h"

int main()
{
	GCompiler.ReadScript("FirstRun.mika");
    return -GCompiler.GetErrorCount();
}

