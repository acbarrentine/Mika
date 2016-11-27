#pragma once

#include "Identifier.h"

class Variable;
class SymbolTable;

class FunctionDeclaration : public ManagedEntity
{
protected:
	int mRootToken;
	Identifier mName;
	class Type* mReturnType;
	std::vector<Variable*> mParameterList;
	bool mTakesLocation;

public:
	FunctionDeclaration(int rootToken)
		: mRootToken(rootToken)
		, mReturnType(nullptr)
		, mTakesLocation(false)
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

	void SetTakesLocation(bool takesLocation) { mTakesLocation = takesLocation; }
	bool GetTakesLocation() const { return mTakesLocation; }

	int GetRootToken() const { return mRootToken; }

	virtual bool IsScriptFunction() const { return false; }
};
