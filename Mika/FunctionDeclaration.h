#pragma once

#include "Identifier.h"

class Variable;
class SymbolTable;

class FunctionDeclaration
{
protected:
	size_t mRootToken;
	Identifier mName;
	class Type* mReturnType;
	std::vector<Variable*> mParameterList;

public:
	FunctionDeclaration(size_t rootToken, Identifier name)
		: mRootToken(rootToken)
		, mName(name)
		, mReturnType(nullptr)
	{}

	void AddParameter(Variable* param)
	{
		mParameterList.push_back(param);
	}

	void BindParameters(SymbolTable& symbolTable);

	void SetReturnType(Type* returnType) { mReturnType = returnType; }
	Type* GetReturnType() const { return mReturnType; }
};
