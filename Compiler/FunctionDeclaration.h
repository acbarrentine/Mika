#pragma once

#include "Identifier.h"
#include "Variable.h"

class SymbolTable;

class FunctionDeclaration : public ManagedEntity
{
protected:
	int mRootToken;
	Identifier mName;
	class Type* mReturnType;
	std::vector<Variable*> mParameterList;

public:
	FunctionDeclaration(int rootToken)
		: mRootToken(rootToken)
		, mReturnType(nullptr)
	{}

	void AddParameter(Variable* param);
	Variable* GetParameter(int index);
	int GetParameterCount() const
	{
		return (int)mParameterList.size();
	}

	void BindParameters(SymbolTable& symbolTable);

	void SetReturnType(Type* returnType) { mReturnType = returnType; }
	Type* GetReturnType() const { return mReturnType; }
	
	virtual void SetName(Identifier name) { mName = name; }
	Identifier GetName() const { return mName; }

	int GetRootToken() const { return mRootToken; }

	virtual bool IsScriptFunction() const { return false; }
};
