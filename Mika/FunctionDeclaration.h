#pragma once

#include "Identifier.h"

class Variable;
class SymbolTable;

class FunctionDeclaration
{
protected:
	int mRootToken;
	bool mScriptFunction;
	Identifier mName;
	class Type* mReturnType;
	std::vector<Variable*> mParameterList;

public:
	FunctionDeclaration(int rootToken, Identifier name)
		: mRootToken(rootToken)
		, mScriptFunction(false)
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
		return (int)mParameterList.size();
	}

	void BindParameters(SymbolTable& symbolTable);

	void SetReturnType(Type* returnType) { mReturnType = returnType; }
	Type* GetReturnType() const { return mReturnType; }
	Identifier GetName() const { return mName; }

	void SetScriptFunction() { mScriptFunction = true; }
	bool IsScriptFunction() const { return mScriptFunction; }
};
