#pragma once

#include "Expression.h"

enum OpCode : int;
enum TType : int;

class BinaryExpression : public Expression
{
protected:
	std::unique_ptr<Expression> mLeft;
	std::unique_ptr<Expression> mRight;
	TType mTokenType;
	OpCode mOpCode;

public:
	BinaryExpression(int rootToken);

	void SetLeft(Expression* expr) { mLeft = std::unique_ptr<Expression>(expr); }
	void SetRight(Expression* expr) { mRight = std::unique_ptr<Expression>(expr); }

	int GetPrecedence() const;

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
