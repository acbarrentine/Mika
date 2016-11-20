#include "stdafx.h"
#include "Catch.hpp"
#include "../Compiler/Compiler.h"
#include "MikaVM.h"

class CompileAndRunFixture
{
public:
	CompileAndRunFixture()
	{
	}

	virtual ~CompileAndRunFixture()
	{
	}

	void CompileAndRun(std::string scriptName)
	{
		GCompiler.Reset();

		std::string glueHeader = "../TestScripts/MikaGlue.mikah";
		std::string glueModule = "Glue.tmp";
		std::string sourceFile = "../TestScripts/" + scriptName + ".mika";
		std::string objectPath = "../TestScripts/Output/" + scriptName + ".miko";
		std::string debugPath = "../TestScripts/Output/" + scriptName + ".mikd";
		std::string testEntry = scriptName + ":TestEntry";

		if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlueHeader(glueHeader.c_str());
		if (GCompiler.GetErrorCount() == 0) GCompiler.ParseGlueHeader();
		if (GCompiler.GetErrorCount() == 0) GCompiler.WriteGlueFile(glueModule.c_str());
		if (GCompiler.GetErrorCount() == 0) GCompiler.ReadScript(sourceFile.c_str());
		if (GCompiler.GetErrorCount() == 0) GCompiler.ParseScript();
		if (GCompiler.GetErrorCount() == 0) GCompiler.AnalyzeScript();
		if (GCompiler.GetErrorCount() == 0) GCompiler.WriteObjectFile(objectPath.c_str(), debugPath.c_str());

		REQUIRE(GCompiler.GetErrorCount() == 0);

		MikaVM vm;
		vm.Import(objectPath.c_str());
		vm.Execute(testEntry.c_str());
	}
};

TEST_CASE_METHOD(CompileAndRunFixture, "FirstRun", "[FirstRun]")
{
	CompileAndRun("FirstRun");
}

TEST_CASE_METHOD(CompileAndRunFixture, "TestMath", "[Math]")
{
	CompileAndRun("TestMath");
}
