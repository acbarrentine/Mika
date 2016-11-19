#pragma once

class Type;
class Statement;
class FunctionDeclaration;
class SymbolTable;
class ObjectFileHelper;

#include "FunctionDeclaration.h"

class ScriptFunction : public FunctionDeclaration
{
protected:
	Statement* mStatement;

public:
	ScriptFunction(int rootToken)
		: FunctionDeclaration(rootToken)
	{}

	virtual void SetName(Identifier name);
	virtual bool IsScriptFunction() const { return true; }

	void SetStatement(Statement* stmt)
	{
		mStatement = stmt;
	}

	void ResolveTypes(SymbolTable& symbolTable);
	void GenCode(ObjectFileHelper& helper);
};
