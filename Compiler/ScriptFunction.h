#pragma once

#include "FunctionDeclaration.h"

class Statement;
class CompoundStatement;
class SymbolTable;
class ObjectFileHelper;
class Label;

class ScriptFunction : public FunctionDeclaration
{
protected:
	CompoundStatement* mBody;
	Label* mEndLabel;
	bool mIsGlobal;

public:
	ScriptFunction(int rootToken, bool isGlobal)
		: FunctionDeclaration(rootToken)
		, mEndLabel(nullptr)
		, mIsGlobal(isGlobal)
	{}

	virtual void SetName(Identifier name);
	virtual bool IsScriptFunction() const { return true; }
	bool IsGlobal() const { return mIsGlobal; }

	void SetStatement(CompoundStatement* stmt)
	{
		mBody = stmt;
	}
	void AddStatement(Statement* stmt);

	void ResolveTypes(SymbolTable& symbolTable);
	void GenCode(ObjectFileHelper& helper);
};
