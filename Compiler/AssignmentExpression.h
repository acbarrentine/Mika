#pragma once

#include "BinaryExpression.h"

class AssignmentExpression : public BinaryExpression
{
public:
	AssignmentExpression(int rootToken);

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
