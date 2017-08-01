#include "stdafx.h"
#include "UserDefinedType.h"
#include "MemberFunctionDeclaration.h"
#include "Compiler.h"

void UserDefinedType::AddMemberFunction(MemberFunctionDeclaration* decl)
{
	MemberFunctionDeclaration* prevDecl = mDeclarations[decl->GetName()];
	if (prevDecl)
	{
		GCompiler.Error(decl->GetRootToken(), "function already declared");
		GCompiler.Error(prevDecl->GetRootToken(), "see previous declaration");
	}

	mOrderedDeclarations.emplace_back(decl);
	mDeclarations[decl->GetMemberName()] = decl;

	if (decl->IsConstructor())
		mHasConstructor = true;
}

MemberFunctionDeclaration* UserDefinedType::FindDeclaration(Identifier id)
{
    return mDeclarations[id];
}
