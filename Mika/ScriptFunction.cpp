#include "stdafx.h"
#include "Identifier.h"
#include "ScriptFunction.h"
#include "FunctionDeclaration.h"
#include "Statement.h"

Type* ScriptFunction::GetReturnType() const
{
	return mDeclaration->GetReturnType();
}

void ScriptFunction::ResolveTypes()
{
	mStatement->ResolveTypes();
}
