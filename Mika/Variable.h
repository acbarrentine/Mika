#pragma once

#include "Identifier.h"

class Variable
{
protected:
	size_t mRootToken;
	Identifier mName;
	class Type* mType;

public:
	Variable(size_t rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
	{}

	void SetName(Identifier name) { mName = name; }
	void SetType(Type* type) { mType = type; }
};
