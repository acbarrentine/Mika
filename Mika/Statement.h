#pragma once

class SymbolTable;

class Statement
{
protected:
	size_t mRootToken;

public:
	Statement(size_t rootToken)
		: mRootToken(rootToken)
	{}

	virtual void ResolveTypes(SymbolTable& symbolTable) = 0;
};
