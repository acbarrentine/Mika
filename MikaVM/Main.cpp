#include "stdafx.h"
#include "MikaVM.h"

#define CATCH_CONFIG_RUNNER
#include "Catch.hpp"

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
	
	return Catch::Session().run(testArgCount, testArgs);
}

