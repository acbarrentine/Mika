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
	bool mStackOffsetAssigned;
	bool mIsGlobal;

public:
	Variable(int rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
		, mReferences(0)
		, mStackOffset(0)
		, mStackOffsetAssigned(false)
		, mIsGlobal(false)
	{}

	void SetName(Identifier name) { mName = name; }
	void SetType(Type* type) { mType = type; }
	void SetIsGlobal(bool global) { mIsGlobal = global; }

	Identifier GetName() const { return mName; }
	Type* GetType() const { return mType; }
	bool GetIsGlobal() const { return mIsGlobal; }
	int GetRootToken() const { return mRootToken; }

	void AddRef() { ++mReferences; }
	int GetRefCount() const { return mReferences; }

	void SetStackOffset(int offset)
	{
		mStackOffset = offset;
		mStackOffsetAssigned = true;
	}
	int GetStackOffset() const { return mStackOffset; }
	bool IsStackOffsetAssigned() const { return mStackOffsetAssigned; }
};
