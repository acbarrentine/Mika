#pragma once

class FunctionDeclaration;
class UserDefinedType;

class GlueGenerator
{
protected:
	std::ofstream mOutStream;
	std::vector<const FunctionDeclaration*> mDeclarations;
	std::vector<const UserDefinedType*> mUserDefinedTypes;
	std::vector<const char*> mGlueFunctions;

public:
	void AddFunctions(std::vector<FunctionDeclaration*>& declarations);
	void AddUserDefinedTypes(std::vector<UserDefinedType*>& userDefinedTypes);
	void WriteGlueFile(const char* glueFileName);

	void AddGlueFunctionName(const char* name);

	std::ofstream& GetStream() { return mOutStream; }
};
