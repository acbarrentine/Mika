#pragma once

class Type;
class SymbolTable;
class ObjectFileHelper;
class IRRegisterOperand;

class Expression
{
protected:
	int mRootToken;
	Type* mType;
	IRRegisterOperand* mResultRegister;
	
public:
	Expression(int rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) = 0;
	virtual void GenCode(ObjectFileHelper& helper) = 0;
	virtual void GenAssign(ObjectFileHelper& helper, IRRegisterOperand* src);

	Type* GetType() const { return mType; }
	int GetRootToken() const { return mRootToken; }
	IRRegisterOperand* GetResultRegister() const { return mResultRegister; }
};
