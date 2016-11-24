#pragma once

#include "Statement.h"

class Expression;
class Variable;

class VariableDeclarationStatement : public Statement
{
protected:
	Expression* mExpression;
	Variable* mVariable;

public:
	VariableDeclarationStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mVariable(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = expr; }
	void SetVariable(Variable* var) { mVariable = var; }
	void SetGlobal(bool isGlobal);

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};


