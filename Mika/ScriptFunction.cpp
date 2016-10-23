#include "stdafx.h"
#include "Identifier.h"
#include "ScriptFunction.h"
#include "FunctionDeclaration.h"
#include "Statement.h"
#include "SymbolTable.h"


Type* ScriptFunction::GetReturnType() const
{
	return mDeclaration->GetReturnType();
}

void ScriptFunction::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.Push();
	mDeclaration->BindParameters(symbolTable);
	mStatement->ResolveTypes(symbolTable);
	symbolTable.Pop();
}
