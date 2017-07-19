#pragma once

class SymbolTable;
class ObjectFileHelper;
class Type;

class Statement : public ManagedEntity
{
protected:
	int mRootToken;

public:
	Statement(int rootToken)
		: mRootToken(rootToken)
	{}

	virtual void ResolveTypes(SymbolTable& symbolTable) = 0;
	virtual void GenCode(ObjectFileHelper& helper) = 0;
	virtual void CheckReturnStatement(Type* expectedType);
};
