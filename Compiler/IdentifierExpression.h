#pragma once

#include "Expression.h"
#include "Identifier.h"

class Variable;

class IdentifierExpression : public Expression
{
protected:
	Identifier mValue;
	Variable* mVariable;

public:
	IdentifierExpression(int rootToken)
		: Expression(rootToken)
		, mVariable(nullptr)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
	virtual void GenAssign(ObjectFileHelper& helper, IROperand* src) override;
};
