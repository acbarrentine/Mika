#pragma once

#include "Identifier.h"

class Type
{
protected:
	Identifier mName;
	const char* mNativeName;
	const char* mCellField;
	int mSize;

public:
	Type(Identifier name, const char* nativeName, const char* cellField, int size)
		: mName(name)
		, mNativeName(nativeName)
		, mCellField(cellField)
		, mSize(size)
	{}

	virtual ~Type() {}
	
	int GetSize() const { return mSize; }
	const char* GetName() const { return mName.GetString(); }
	const char* GetNativeName() const { return mNativeName; }
	const char* GetCellField() const { return mCellField; }
};
