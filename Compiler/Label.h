#pragma once

class IRScope;

class Label : public ManagedEntity
{
protected:
	Identifier mName;
	IRScope* mScope;
	int mSequence;
	int mByteCodeOffset;

public:
	Label(Identifier name, int sequence)
		: mName(name)
		, mScope(nullptr)
		, mSequence(sequence)
		, mByteCodeOffset(0)
	{}

	Identifier GetName() const { return mName; }
	int GetSequence() const { return mSequence; }

	int GetByteCodeOffset() const { return mByteCodeOffset; }
	void SetByteCodeOffset(int offset) { mByteCodeOffset = offset; }
	
	void SetScope(IRScope* scope) { mScope = scope; }
	IRScope* GetScope() const { return mScope; }
};
