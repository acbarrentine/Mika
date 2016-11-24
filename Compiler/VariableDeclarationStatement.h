#pragma once

#include "Statement.h"
#include "Expression.h"
#include "Variable.h"

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

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};


