#include "stdafx.h"
#include "SymbolTable.h"
#include "Variable.h"


void SymbolTable::AddVariable(Variable* var)
{
	VariableMap& currentFrame = mLookup.back();
	currentFrame.insert(std::make_pair(var->GetName(), var));
}

Variable* SymbolTable::FindVariable(Identifier id)
{
	for (std::vector<VariableMap>::reverse_iterator it = mLookup.rbegin(); it != mLookup.rend(); ++it)
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
