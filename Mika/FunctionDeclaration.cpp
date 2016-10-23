#include "stdafx.h"
#include "FunctionDeclaration.h"
#include "SymbolTable.h"

void FunctionDeclaration::BindParameters(SymbolTable& symbolTable)
{
	for (Variable* var : mParameterList)
	{
		symbolTable.AddVariable(var);
	}
}
