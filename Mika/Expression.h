#pragma once

class Type;

class Expression
{
protected:
	size_t mRootToken;
	Type* mType;
	
public:
	Expression(size_t rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
	{}

	virtual void ResolveType() = 0;
	Type* GetType() const { return mType; }
};
