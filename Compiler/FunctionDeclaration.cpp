#include "stdafx.h"
#include "FunctionDeclaration.h"
#include "SymbolTable.h"

void FunctionDeclaration::AddParameter(Variable* param)
{
	mParameterList.push_back(param);
}

Variable* FunctionDeclaration::GetParameter(int index)
{
	return mParameterList[index];
}

void FunctionDeclaration::BindParameters(SymbolTable& symbolTable)
{
	for (auto& var : mParameterList)
	{
		symbolTable.AddVariable(var);
	}
}
