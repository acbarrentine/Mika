#pragma once

#include "Identifier.h"

class Variable
{
protected:
	int mRootToken;
	Identifier mName;
	class Type* mType;
	int mReferences;
	int mStackOffset;

public:
	Variable(int rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
		, mReferences(0)
		, mStackOffset(0)
	{}

	void SetName(Identifier name) { mName = name; }
	void SetType(Type* type) { mType = type; }

	Identifier GetName() const { return mName; }
	Type* GetType() const { return mType; }

	int GetRootToken() const { return mRootToken; }

	void AddRef() { ++mReferences; }
	int GetRefCount() const { return mReferences; }

	void SetStackOffset(int offset) { mStackOffset = offset; }
	int GetStackOffset() const { return mStackOffset; }
};
