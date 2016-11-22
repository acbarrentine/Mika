#pragma once

class Type;
class SymbolTable;
class ObjectFileHelper;
class IROperand;

class Expression
{
protected:
	int mRootToken;
	Type* mType;
	IROperand* mResultRegister;
	
public:
	Expression(int rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) = 0;
	virtual void GenCode(ObjectFileHelper& helper) = 0;
	virtual void GenAssign(ObjectFileHelper& helper, IROperand* src);

	Type* GetType() const { return mType; }
	int GetRootToken() const { return mRootToken; }
	IROperand* GetResultRegister() const { return mResultRegister; }
};
