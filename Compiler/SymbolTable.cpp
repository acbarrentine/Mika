#include "stdafx.h"
#include "SymbolTable.h"
#include "Variable.h"
#include "Label.h"

void SymbolTable::AddVariable(Variable* var)
{
	Identifier name = var->GetName();
	if (var->GetIsGlobal())
	{
		VariableMap& globalFrame = mVariableLookup.front();
		assert(globalFrame.find(name) == globalFrame.end());
		globalFrame[name] = var;
	}
	else
	{
		VariableMap& currentFrame = mVariableLookup.back();
		assert(currentFrame.find(name) == currentFrame.end());
		currentFrame[name] = var;
	}
}

Variable* SymbolTable::FindVariable(Identifier id)
{
	for (std::vector<VariableMap>::reverse_iterator it = mVariableLookup.rbegin(); it != mVariableLookup.rend(); ++it)
	{
		VariableMap& symbols = *it;
		VariableMap::iterator found = symbols.find(id);
		if (found != symbols.end())
		{
			return found->second;
		}
	}
	return nullptr;
}

Label* SymbolTable::GenLabel(Identifier name, int sequence)
{
	Label* label = new Label(name, sequence);
	LabelMap& currentFrame = mLabelLookup.back();
	assert(currentFrame.find(name) == currentFrame.end());
	currentFrame[name] = label;
	return label;
}

Label* SymbolTable::FindLabel(Identifier id)
{
	for (std::vector<LabelMap>::reverse_iterator it = mLabelLookup.rbegin(); it != mLabelLookup.rend(); ++it)
	{
		LabelMap& symbols = *it;
		LabelMap::iterator found = symbols.find(id);
		if (found != symbols.end())
		{
			return found->second;
		}
	}
	return nullptr;
}
