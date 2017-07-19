#include "stdafx.h"
#include "../Compiler/Compiler.h"

void Usage()
{
    GCompiler.Message(MsgSeverity::kInfo, "Usage: MikaCL <glue_source_header_path> [-s <source_script_path>] [-o <output_file_path>] [-d <debug_file_path>]\n");
    GCompiler.Message(MsgSeverity::kInfo, "  Example: MikaCL TestScrits/MikaGlue.mikah -s TestScripts/FirstRun.mika -o TestScripts/Output/FirstRun.miko\n");
	GCompiler.Message(MsgSeverity::kInfo, "Or, to generate glue:\n");
	GCompiler.Message(MsgSeverity::kInfo, "  MikaCL -glue <glue_source_header_path> <generated_native_glue_path>\n");
}

void MakeGlue(const char* glueSource, const char* generatedGlue)
{
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlueHeader(glueSource);
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseGlueHeader();
	if (GCompiler.GetErrorCount() == 0) GCompiler.WriteGlueFile(generatedGlue);
}

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        Usage();
        return -1;
    }
    
	if (strncmp(argv[1], "-glue", 5) == 0)
	{
		if (argc < 4)
		{
			Usage();
			return -1;
		}
		MakeGlue(argv[2], argv[3]);
		return 0;
	}

	std::string glueHeaderPath(argv[1]);
	std::string sourcePath;
	std::string objectPath;
	std::string debugPath;

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

                default:
					++argIndex;
					break;
            }
        }
		else
		{
			GCompiler.Message(MsgSeverity::kInfo, "Unknown command line option: '%s'\n", argv[argIndex]);
			Usage();
			return -1;
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
    if (GCompiler.GetErrorCount() == 0) GCompiler.ReadScript(sourcePath.c_str());
    if (GCompiler.GetErrorCount() == 0) GCompiler.ParseScript();
    if (GCompiler.GetErrorCount() == 0) GCompiler.AnalyzeScript();
    if (GCompiler.GetErrorCount() == 0) GCompiler.WriteObjectFile(objectPath.c_str(), debugPath.c_str());
    return -GCompiler.GetErrorCount();
}
