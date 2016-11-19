#include "stdafx.h"
#include "Compiler.h"

void Usage()
{
	GCompiler.Message(MsgSeverity::kInfo, "Usage: mika <glue_header_path> [-s <source_script_path>] [-o <output_file_path>] [-d <debug_file_path>] [-g <glue_module_path>]\n");
	GCompiler.Message(MsgSeverity::kInfo, "  Example: mika TestScripts/MikaGlue.mikah -s TestScripts/FirstRun.mika -o TestScripts/Output/FirstRun.miko\n");
}

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		Usage();
		return -1;
	}

	std::string glueHeaderPath(argv[1]);
	std::string sourcePath;
	std::string objectPath;
	std::string debugPath;
	std::string glueModulePath;

	int argIndex = 2;
	while (argIndex < argc)
	{
		if ('-' == argv[argIndex][0])
		{
			switch (argv[argIndex][1])
			{
				case 's':
					sourcePath = argv[argIndex + 1];
					argIndex += 2;
					break;

				case 'o':
					objectPath = argv[argIndex + 1];
					argIndex += 2;
					break;

				case 'd':
					debugPath = argv[argIndex + 1];
					argIndex += 2;
					break;

				case 'g':
					glueModulePath = argv[argIndex + 1];
					argIndex += 2;
					break;

				default:
					++argIndex;
					break;
			}
		}
	}

	if (!glueHeaderPath.length()
		|| (sourcePath.length() && !objectPath.length()))
	{
		Usage();
		return -1;
	}

	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlueHeader(glueHeaderPath.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseGlueHeader();
	if (GCompiler.GetErrorCount() == 0) GCompiler.WriteGlueFile(glueModulePath.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadScript(sourcePath.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseScript();
	if (GCompiler.GetErrorCount() == 0) GCompiler.AnalyzeScript();
	if (GCompiler.GetErrorCount() == 0) GCompiler.WriteObjectFile(objectPath.c_str(), debugPath.c_str());
	return -GCompiler.GetErrorCount();
}

