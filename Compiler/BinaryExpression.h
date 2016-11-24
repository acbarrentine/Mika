#pragma once

#include "Expression.h"

enum OpCode : int;
enum TType : int;

class BinaryExpression : public Expression
{
protected:
	Expression* mLeft;
	Expression* mRight;
	TType mTokenType;
	OpCode mOpCode;

public:
	BinaryExpression(int rootToken);

	void SetLeft(Expression* expr) { mLeft = expr; }
	void SetRight(Expression* expr) { mRight = expr; }

	int GetPrecedence() const;

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
