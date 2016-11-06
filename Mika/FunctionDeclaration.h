#pragma once

#include "Identifier.h"

class Variable;
class SymbolTable;

class FunctionDeclaration
{
protected:
	int mRootToken;
	Identifier mName;
	class Type* mReturnType;
	std::vector<Variable*> mParameterList;

public:
	FunctionDeclaration(int rootToken, Identifier name)
		: mRootToken(rootToken)
		, mName(name)
		, mReturnType(nullptr)
	{}

	void AddParameter(Variable* param)
	{
		mParameterList.push_back(param);
	}
	Variable* GetParameter(int index)
	{
		return mParameterList[index];
	}
	int GetParameterCount() const
	{
		return mParameterList.size();
	}

	void BindParameters(SymbolTable& symbolTable);

	void SetReturnType(Type* returnType) { mReturnType = returnType; }
	Type* GetReturnType() const { return mReturnType; }
	Identifier GetName() const { return mName; }
};
