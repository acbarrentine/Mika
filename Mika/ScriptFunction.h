#pragma once

class Type;
class Statement;

class ScriptFunction
{
protected:
	size_t mRootToken;
	Identifier mName;
	Type* mReturnType;
	Statement* mStatement;

public:
	ScriptFunction(size_t rootToken)
		: mRootToken(rootToken)
		, mReturnType(nullptr)
	{}

	void SetName(Identifier name) { mName = name; }
	Identifier GetName() const { return mName; }

	void SetReturnType(Type* returnType) { mReturnType = returnType; }
	Type* GetReturnType() const { return mReturnType; }

	void SetStatement(Statement* stmt)
	{
		mStatement = stmt;
	}
};
