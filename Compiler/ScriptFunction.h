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
	std::unique_ptr<Statement> mStatement;

public:
	ScriptFunction(int rootToken)
		: FunctionDeclaration(rootToken)
	{}

	virtual void SetName(Identifier name);
	virtual bool IsScriptFunction() const { return true; }

	void SetStatement(Statement* stmt)
	{
		mStatement = std::unique_ptr<Statement>(stmt);
	}

	void ResolveTypes(SymbolTable& symbolTable);
	void GenCode(ObjectFileHelper& helper);
};
