#pragma once

#include "Expression.h"

class BinaryExpression : public Expression
{
protected:
	Expression* mLeft;
	Expression* mRight;

public:
	BinaryExpression(size_t rootToken)
		: Expression(rootToken)
	{
	}

	void SetLeft(Expression* expr) { mLeft = expr; }
	void SetRight(Expression* expr) { mRight = expr; }

	int GetPrecedence() const;

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
