#pragma once

class Type;
class SymbolTable;
class ObjectFileHelper;
class IRRegisterOperand;

class Expression
{
protected:
	size_t mRootToken;
	Type* mType;
	IRRegisterOperand* mResultRegister;
	
public:
	Expression(size_t rootToken)
		: mRootToken(rootToken)
		, mType(nullptr)
	{}

	virtual void ResolveType(SymbolTable& symbolTable) = 0;
	virtual void GenCode(ObjectFileHelper& helper) = 0;

	Type* GetType() const { return mType; }
	size_t GetRootToken() const { return mRootToken; }
	IRRegisterOperand* GetResultRegister() const { return mResultRegister; }
};
