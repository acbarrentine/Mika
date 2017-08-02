#include "stdafx.h"
#include "MemberFunctionCallExpression.h"
#include "UserDefinedType.h"
#include "Compiler.h"
#include "MemberFunctionDeclaration.h"
#include "ObjectFileHelper.h"

void MemberFunctionCallExpression::ResolveType(SymbolTable& symbolTable)
{
    mObject->ResolveType(symbolTable);
    
    Type* rawObjectType = mObject->GetType();
    UserDefinedType* objectType = dynamic_cast<UserDefinedType*>(rawObjectType);
    if (!objectType)
    {
        GCompiler.Error(mObject->GetRootToken(), "user defined type expected");
        mType = GCompiler.GetIntType();
        return;
    }
    
    Token& nameToken = GCompiler.GetToken(mRootToken);
    mMemberDeclaration = objectType->FindDeclaration(nameToken.GetIdentifier());
    
    if (!mMemberDeclaration)
    {
        GCompiler.Error(mRootToken, "member function declaration not found");
        mType = GCompiler.GetIntType();
        return;
    }

    mType = mMemberDeclaration->GetReturnType();
    mDeclaration = mMemberDeclaration;
    ResolveActuals(symbolTable);
}

void MemberFunctionCallExpression::GenCode(ObjectFileHelper& helper)
{
    if (!mMemberDeclaration->IsConstructor())
    {
        mObject->GenCode(helper);
    }
    
    for (Expression* expr : mActuals)
    {
        expr->GenCode(helper);
    }

    int reset = 1;
    if (!mMemberDeclaration->IsConstructor())
    {
        // push object node
        IRInstruction* pushOp = helper.EmitInstruction(PushArgument, mObject->GetRootToken());
        pushOp->SetOperand(0, mObject->GetResultRegister());
        pushOp->SetOperand(1, new IRIntOperand(reset));
        reset = 0;
    }
    
    for (size_t i = 0; i < mActuals.size(); ++i)
    {
        Expression* expr = mActuals[i];
        IRInstruction* pushOp = helper.EmitInstruction(PushArgument, expr->GetRootToken());
        pushOp->SetOperand(0, expr->GetResultRegister());
        pushOp->SetOperand(1, new IRIntOperand(reset));
        reset = 0;
    }
    
    IRInstruction* callOp = helper.EmitInstruction(CallNativeFunction, mRootToken);
    callOp->SetOperand(0, new IRFunctionOperand(mDeclaration));
    
    if (mType != GCompiler.GetVoidType())
    {
        mResultRegister = new IRRegisterOperand;
        
        IRInstruction* copyOp = helper.EmitInstruction(CopyResultRegister, mRootToken);
        copyOp->SetOperand(0, mResultRegister);
    }
}
