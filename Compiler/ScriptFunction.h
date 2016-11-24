#pragma once

class Type;
class Statement;
class FunctionDeclaration;
class SymbolTable;
class ObjectFileHelper;

#include "FunctionDeclaration.h"
#include "Statement.h"

class ScriptFunction : public FunctionDeclaration
{
protected:
	Statement* mStatement;
	bool mIsGlobal;

public:
	ScriptFunction(int rootToken, bool isGlobal)
		: FunctionDeclaration(rootToken)
		, mIsGlobal(isGlobal)
	{}

	virtual void SetName(Identifier name);
	virtual bool IsScriptFunction() const { return true; }
	bool IsGlobal() const { return mIsGlobal; }

	void SetStatement(Statement* stmt)
	{
		mStatement = stmt;
	}

	void ResolveTypes(SymbolTable& symbolTable);
	void GenCode(ObjectFileHelper& helper);
};
