#pragma once

#include "Expression.h"
class FunctionDeclaration;

class FunctionCallExpression : public Expression
{
protected:
	std::vector<Expression*> mActuals;
	FunctionDeclaration* mDeclaration;

public:
	FunctionCallExpression(size_t rootToken)
		: Expression(rootToken)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;

	void AddActual(Expression* arg)
	{
		mActuals.push_back(arg);
	}
};
