#include "stdafx.h"
#include "Compiler.h"

void Usage()
{
	GCompiler.Message(MsgSeverity::kInfo, "Usage: mika <glue_file_path> <source_script_path> <output_file_path>\n");
	GCompiler.Message(MsgSeverity::kInfo, "  Example: mika TestScripts/MikaGlue.mikah TestScripts/FirstRun.mika TestScripts/Output/FirstRun.miko\n");
}

int main(int argc, const char* argv[])
{
	if (argc < 4)
	{
		Usage();
		return -1;
	}

	std::string gluePath(argv[1]);
	std::string sourcePath(argv[2]);
	std::string objectPath(argv[3]);
	
	std::string debugFilePath(objectPath);
	debugFilePath[debugFilePath.length() - 1] = 'd';

	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlue(gluePath.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseGlue();
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadScript(sourcePath.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseScript();
	if (GCompiler.GetErrorCount() == 0) GCompiler.AnalyzeScript();
	if (GCompiler.GetErrorCount() == 0) GCompiler.WriteObjectFile(objectPath.c_str(), debugFilePath.c_str());
	return -GCompiler.GetErrorCount();
}

