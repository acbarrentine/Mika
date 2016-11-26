#pragma once

class Label : public ManagedEntity
{
protected:
	Identifier mName;
	int mSequence;
	int mByteCodeOffset;

public:
	Label(Identifier name, int sequence)
		: mName(name)
		, mSequence(sequence)
		, mByteCodeOffset(0)
	{}

	Identifier GetName() const { return mName; }
	int GetSequence() const { return mSequence; }

	int GetByteCodeOffset() const { return mByteCodeOffset; }
	void SetByteCodeOffset(int offset) { mByteCodeOffset = offset; }
};
