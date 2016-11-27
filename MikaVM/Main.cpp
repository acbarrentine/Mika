#include "stdafx.h"
#include "MikaVM.h"
#include "../Compiler/Compiler.h"

#define CATCH_CONFIG_RUNNER
#include "Catch.hpp"

bool UpdateGlue()
{
	std::string glueHeader = "../TestScripts/MikaGlue.mikah";
	std::string tmpGlueModule = "GeneratedGlue.tmp";
	std::string curGlueModule = "GeneratedGlue.hpp";

	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlueHeader(glueHeader.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseGlueHeader();
	if (GCompiler.GetErrorCount() == 0) GCompiler.WriteGlueFile(tmpGlueModule.c_str());

	std::ifstream tempHandle(tmpGlueModule.c_str(), std::ios::ate | std::ios::binary);
	std::ifstream curHandle(curGlueModule.c_str(), std::ios::ate | std::ios::binary);
	return tempHandle.tellg() == curHandle.tellg();
}

int main(int argc, char* argv[])
{
	char* testArgs[32] =
	{
		"MikaVM.exe",
		"--out", "%debug",
		"--durations", "yes",
		"--break",
	};
	int testArgCount = 6;
	for (int argIndex = 1; argIndex < argc; ++argIndex)
	{
		testArgs[testArgCount++] = argv[argIndex];
	}
	
	if (!UpdateGlue())
	{
		OutputDebugString("Glue code changed. Merge and recompile.\n");
		return -1;
	}

	return Catch::Session().run(testArgCount, testArgs);
}

