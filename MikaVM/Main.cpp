#include "stdafx.h"
#include "MikaVM.h"

void Usage()
{
	std::cout << "Usage: MikaVM <miko_object_file_path> <FunctionName> <results_file_path>" << std::endl;
	std::cout << "  Example: MikaVM TestScripts/Output/FirstRun.miko FirstRun:TestEntry TestScripts/Output/FirstRun.results" << std::endl;
}

int main(int argc, const char* argv[])
{
	if (argc < 4)
	{
		Usage();
		return -1;
	}

	std::string objectPath(argv[1]);
	std::string functionName(argv[2]);
	std::string resultsPath(argv[3]);

	MikaVM vm;
	vm.Import(objectPath.c_str());
	vm.Execute(functionName.c_str());

	std::fstream resultsFile(resultsPath.c_str(), std::ios::out);
	if (resultsFile)
	{
		resultsFile << "Passed" << std::endl;
	}

	return 0;
}

