#pragma once

class Type;
class Statement;
class CompoundStatement;
class FunctionDeclaration;
class SymbolTable;
class ObjectFileHelper;

#include "FunctionDeclaration.h"

class ScriptFunction : public FunctionDeclaration
{
protected:
	CompoundStatement* mStatement;
	bool mIsGlobal;

public:
	ScriptFunction(int rootToken, bool isGlobal)
		: FunctionDeclaration(rootToken)
		, mIsGlobal(isGlobal)
	{}

	virtual void SetName(Identifier name);
	virtual bool IsScriptFunction() const { return true; }
	bool IsGlobal() const { return mIsGlobal; }

	void SetStatement(CompoundStatement* stmt)
	{
		mStatement = stmt;
	}
	void AddStatement(Statement* stmt);

	void ResolveTypes(SymbolTable& symbolTable);
	void GenCode(ObjectFileHelper& helper);
};
