#pragma once

#include "Identifier.h"

class Variable;
class Label;

class SymbolTable
{
protected:
	typedef std::map<Identifier, Variable*> VariableMap;
	std::vector<VariableMap> mVariableLookup;

	typedef std::map<Identifier, Label*> LabelMap;
	std::vector<LabelMap> mLabelLookup;

	int mLabelSequence;

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
		mVariableLookup.push_back(VariableMap());
		mLabelLookup.push_back(LabelMap());
		mLabelSequence = 1;
	}

	void Pop()
	{
		mVariableLookup.pop_back();
		mLabelLookup.pop_back();
	}

	void AddVariable(Variable* var);
	Variable* FindVariable(Identifier id);

	int GetNextLabelSequence() { return mLabelSequence++; }
	Label* GenLabel(Identifier name, int sequence);
	Label* FindLabel(Identifier id);
};
