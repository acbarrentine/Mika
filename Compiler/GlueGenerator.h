#pragma once

class FunctionDeclaration;

class GlueGenerator
{
protected:
	std::vector<FunctionDeclaration*> mDeclarations;

public:
	void AddFunction(FunctionDeclaration* decl);
	void WriteGlueFile(const char* glueFileName);
};
