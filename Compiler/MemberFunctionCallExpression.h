#pragma once

#include "FunctionCallExpression.h"

class UserDefinedType;
class MemberFunctionDeclaration;

class MemberFunctionCallExpression : public FunctionCallExpression
{
protected:
    Expression* mObject;
    UserDefinedType* mParentType;
    MemberFunctionDeclaration* mMemberDeclaration;
    
public:
    MemberFunctionCallExpression(int rootToken, Expression* obj)
        : FunctionCallExpression(rootToken)
        , mObject(obj)
        , mParentType(nullptr)
        , mMemberDeclaration(nullptr)
    {}
    
    virtual void ResolveType(SymbolTable& symbolTable) override;
    virtual void GenCode(ObjectFileHelper& helper) override;
};
