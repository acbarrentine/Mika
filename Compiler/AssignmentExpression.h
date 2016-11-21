#pragma once

#include "Expression.h"
#include "Variable.h"

enum OpCode : int;
enum TType : int;

class AssignmentExpression : public Expression
{
protected:
	std::unique_ptr<Variable> mLeft;
	std::unique_ptr<Expression> mRight;
	TType mTokenType;
	OpCode mOpCode;

public:
	AssignmentExpression(int rootToken);

	void SetLeft(Variable* var) { mLeft = std::unique_ptr<Variable>(var); }
	void SetRight(Expression* expr) { mRight = std::unique_ptr<Expression>(expr); }

	int GetPrecedence() const;

	virtual void ResolveType(SymbolTable& symbolTable) override;
	virtual void GenCode(ObjectFileHelper& helper) override;
};
