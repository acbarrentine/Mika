#include "stdafx.h"
#include "LogicalExpression.h"
#include "SymbolTable.h"
#include "Compiler.h"
#include "ObjectFileHelper.h"

void LogicalExpression::ResolveType(SymbolTable& symbolTable)
{
	symbolTable.Push();

	int seq = symbolTable.GetNextLabelSequence();
	mFalseLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("false"), seq);
	mTrueLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("true"), seq);
	mEndLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("done"), seq);

	mLeft->ResolveType(symbolTable);
	mRight->ResolveType(symbolTable);

	Type* intType = GCompiler.FindType(TType::kInt);
	if (mLeft->GetType() != intType)
	{
		GCompiler.Error(mLeft->GetRootToken(), "incompatible type for logical operator");
	}
	if (mRight->GetType() != intType)
	{
		GCompiler.Error(mRight->GetRootToken(), "incompatible type for logical operator");
	}

	mOpCode = (mTokenType == TType::kAnd ? NotEqualsInt : EqualsInt);
	mType = intType;

	symbolTable.Pop();
}

void LogicalExpression::GenCode(ObjectFileHelper& helper)
{
	bool logicalAnd = mTokenType == TType::kAnd;

	mLeft->GenCode(helper);

	mResultRegister = new IRRegisterOperand();

	IRRegisterOperand* zeroRegister = new IRRegisterOperand();
	IRInstruction* zero = helper.EmitInstruction(CopyConstantToStack, mRootToken);
	zero->SetOperand(0, zeroRegister);
	zero->SetOperand(1, new IRIntOperand(0));

	IRInstruction* op1 = helper.EmitInstruction(mOpCode, mRootToken);
	IRRegisterOperand* op1Result = new IRRegisterOperand();
	op1->SetOperand(0, op1Result);
	op1->SetOperand(1, mLeft->GetResultRegister());
	op1->SetOperand(2, zeroRegister);

	IRInstruction* branch1 = helper.EmitInstruction(ConditionalBranch, mRootToken);
	branch1->SetOperand(0, op1Result);
	branch1->SetOperand(1, new IRLabelOperand(logicalAnd ? mFalseLabel : mTrueLabel));

	mRight->GenCode(helper);

	IRInstruction* op2 = helper.EmitInstruction(mOpCode, mRootToken);
	IRRegisterOperand* op2Result = new IRRegisterOperand();
	op2->SetOperand(0, op2Result);
	op2->SetOperand(1, mRight->GetResultRegister());
	op2->SetOperand(2, zeroRegister);

	IRInstruction* branch2 = helper.EmitInstruction(ConditionalBranch, mRootToken);
	branch2->SetOperand(0, op2Result);
	branch2->SetOperand(1, new IRLabelOperand(logicalAnd ? mFalseLabel : mTrueLabel));

	if (logicalAnd)
	{
		helper.EmitLabel(mTrueLabel, mRootToken);
	}
	else
	{
		helper.EmitLabel(mFalseLabel, mRootToken);
	}

	IRInstruction* op3 = helper.EmitInstruction(CopyConstantToStack, mRootToken);
	op3->SetOperand(0, mResultRegister);
	op3->SetOperand(1, new IRIntOperand(logicalAnd ? 1 : 0));

	IRInstruction* branch3 = helper.EmitInstruction(UnconditionalBranch, mRootToken);
	branch3->SetOperand(0, new IRLabelOperand(mEndLabel));

	if (logicalAnd)
	{
		helper.EmitLabel(mFalseLabel, mRootToken);
	}
	else
	{
		helper.EmitLabel(mTrueLabel, mRootToken);
	}

	IRInstruction* op4 = helper.EmitInstruction(CopyConstantToStack, mRootToken);
	op4->SetOperand(0, mResultRegister);
	op4->SetOperand(1, new IRIntOperand(logicalAnd ? 0 : 1));

	helper.EmitLabel(mEndLabel, mRootToken);
}
