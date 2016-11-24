#pragma once

#include "Identifier.h"
class Variable;

class SymbolTable
{
protected:
	typedef std::map<Identifier, Variable*> VariableMap;
	std::vector<VariableMap> mLookup;

public:
	SymbolTable()
	{
		Push();
	}
	~SymbolTable()
	{
		Pop();
	}

	void Push()
	{
		mLookup.push_back(VariableMap());
	}

	void Pop()
	{
		mLookup.pop_back();
	}

	void AddVariable(Variable* var);
	Variable* FindVariable(Identifier id);
};
