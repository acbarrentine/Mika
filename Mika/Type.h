#pragma once

#include "Identifier.h"

class Type
{
protected:
	Identifier mName;
	size_t mSize;

	Type(Identifier name, size_t size)
		: mName(name)
		, mSize(size)
	{}
};
