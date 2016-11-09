#pragma once

class Type;
class Statement;
class FunctionDeclaration;
class SymbolTable;
class ObjectFileHelper;

class ScriptFunction
{
protected:
	int mRootToken;
	Identifier mName;
	FunctionDeclaration* mDeclaration;
	Statement* mStatement;

public:
	ScriptFunction(int rootToken)
		: mRootToken(rootToken)
		, mDeclaration(nullptr)
	{}

	void SetName(Identifier name) { mName = name; }
	Identifier GetName() const { return mName; }
	void SetDeclaration(FunctionDeclaration* decl) { mDeclaration = decl; }
	Type* GetReturnType() const;
	int GetRootToken() const { return mRootToken; }

	void SetStatement(Statement* stmt)
	{
		mStatement = stmt;
	}

	void ResolveTypes(SymbolTable& symbolTable);
	void GenCode(ObjectFileHelper& helper);
};
