#pragma once

#include "Expression.h"

class FunctionDeclaration;

class FunctionCallExpression : public Expression
{
protected:
	std::vector<Expression*> mActuals;
	FunctionDeclaration* mDeclaration;

public:
	FunctionCallExpression(int rootToken)
		: Expression(rootToken)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	void ResolveActuals(SymbolTable& symbolTable);

	virtual void GenCode(ObjectFileHelper& helper) override;

	void AddActual(Expression* arg)
	{
		mActuals.push_back(arg);
	}
};
