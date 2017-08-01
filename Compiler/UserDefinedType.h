#pragma once

#include "Type.h"

class MemberFunctionDeclaration;
class GlueGenerator;

class UserDefinedType : public Type
{
protected:
	typedef std::map<Identifier, MemberFunctionDeclaration*> FunctionDeclarationMap;

	std::vector<MemberFunctionDeclaration*> mOrderedDeclarations;
	FunctionDeclarationMap mDeclarations;
	int mRootToken;
	bool mHasConstructor;

public:
	UserDefinedType(int startToken, Identifier name, const char* nativeName)
		: Type(name, nativeName, "mPtrVal", sizeof(int*))
		, mRootToken(startToken)
		, mHasConstructor(false)
	{}

	void AddMemberFunction(MemberFunctionDeclaration* decl);
    MemberFunctionDeclaration* FindDeclaration(Identifier id);

    bool HasConstructor() const { return mHasConstructor; }

	void GenerateGlue(GlueGenerator& gen) const;
};
