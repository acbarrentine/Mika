// Mika.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Compiler.h"

int main()
{
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlue("MikaGlue.mikah");
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadScript("FirstRun.mika");
    return -GCompiler.GetErrorCount();
}
