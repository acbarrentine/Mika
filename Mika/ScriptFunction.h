#pragma once

class Type;
class Statement;
class FunctionDeclaration;
class SymbolTable;

class ScriptFunction
{
protected:
	size_t mRootToken;
	Identifier mName;
	FunctionDeclaration* mDeclaration;
	Statement* mStatement;

public:
	ScriptFunction(size_t rootToken)
		: mRootToken(rootToken)
		, mDeclaration(nullptr)
	{}

	void SetName(Identifier name) { mName = name; }
	Identifier GetName() const { return mName; }
	void SetDeclaration(FunctionDeclaration* decl) { mDeclaration = decl; }
	Type* GetReturnType() const;

	void SetStatement(Statement* stmt)
	{
		mStatement = stmt;
	}

	void ResolveTypes(SymbolTable& symbolTable);
};
