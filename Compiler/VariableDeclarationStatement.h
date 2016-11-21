#pragma once

#include "Statement.h"
#include "Expression.h"
#include "Variable.h"

class VariableDeclarationStatement : public Statement
{
protected:
	std::unique_ptr<Expression> mExpression;
	std::unique_ptr<Variable> mVariable;

public:
	VariableDeclarationStatement(int rootToken)
		: Statement(rootToken)
		, mExpression(nullptr)
		, mVariable(nullptr)
	{}

	void SetExpression(Expression* expr) { mExpression = std::unique_ptr<Expression>(expr); }
	void SetVariable(Variable* var) { mVariable = std::unique_ptr<Variable>(var); }

	virtual void ResolveTypes(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};


