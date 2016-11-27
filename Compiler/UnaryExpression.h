#pragma once

#include "Expression.h"

enum OpCode : int;

class UnaryExpression : public Expression
{
protected:
	Expression* mTarget;
	OpCode mOpCode;

public:
	UnaryExpression(int rootToken);
	void SetTarget(Expression* expr) { mTarget = expr; }

	void ResolveType(SymbolTable& symbolTable) override;
	void GenCode(ObjectFileHelper& helper) override;
};
