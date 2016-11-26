#pragma once

#include "BinaryExpression.h"

class Label;

class LogicalExpression : public BinaryExpression
{
	Label* mFalseLabel;
	Label* mTrueLabel;
	Label* mEndLabel;

public:
	LogicalExpression(int rootToken) : BinaryExpression(rootToken) {}

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
