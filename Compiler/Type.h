#pragma once

#include "Identifier.h"

class Type
{
protected:
	Identifier mName;
	int mSize;

public:
	Type(Identifier name, int size)
		: mName(name)
		, mSize(size)
	{}

	int GetSize() const { return mSize; }
};
