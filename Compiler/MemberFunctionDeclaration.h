#pragma once

#include "FunctionDeclaration.h"
class GlueGenerator;
class UserDefinedType;

class MemberFunctionDeclaration : public FunctionDeclaration
{
protected:
	UserDefinedType* mParentType;
	Identifier mMemberName;
	bool mIsConstructor;
	bool mDestroyReturnType;

public:
	MemberFunctionDeclaration(int rootToken, UserDefinedType* parentType)
		: FunctionDeclaration(rootToken)
		, mParentType(parentType)
		, mIsConstructor(false)
	{}

	bool IsConstructor() const { return mIsConstructor; }
	void SetConstructor(bool constructor) { mIsConstructor = constructor; }
	void SetDestroyReturnValue(bool destroyReturn) { mDestroyReturnType = destroyReturn; }
	void SetMemberName(Identifier name) { mMemberName = name; }

	virtual void GenerateGlue(GlueGenerator& gen) const override;
};
