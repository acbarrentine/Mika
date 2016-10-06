#pragma once

#include "Identifier.h"

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

	void SetReturnType(Type* returnType) { mReturnType = returnType; }
};
