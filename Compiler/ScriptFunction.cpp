#include "stdafx.h"
#include "Identifier.h"
#include "ScriptFunction.h"
#include "FunctionDeclaration.h"
#include "CompoundStatement.h"
#include "Variable.h"
#include "SymbolTable.h"
#include "ObjectFileHelper.h"
#include "Token.h"
#include "Compiler.h"

void ScriptFunction::SetName(Identifier name)
{
	Token& tok = GCompiler.GetToken(mRootToken);
	Identifier fileNameStem = GCompiler.GetStemName(tok.GetFileIndex());
	mName = GCompiler.ComposeIdentifier("%s:%s", fileNameStem.GetString(), name.GetString());
}

void ScriptFunction::AddStatement(Statement* stmt)
{
	mBody->AddStatement(stmt);
}

void ScriptFunction::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.StartFunction();
	symbolTable.Push();

	mEndLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("end"), symbolTable.GetNextLabelSequence());

	BindParameters(symbolTable);
	mBody->ResolveTypes(symbolTable);
	
	symbolTable.Pop();
}

void ScriptFunction::GenCode(ObjectFileHelper& helper)
{
	IRInstruction* pushScope = helper.EmitInstruction(MoveStackPointer, mRootToken);
	pushScope->SetOperand(0, new IRStackBytesOperand(false));

	// save called arguments
	for (int i = 0; i < GetParameterCount(); ++i)
	{
		Variable* var = GetParameter(i);
		IRInstruction* op = helper.EmitInstruction(CopyArgToStack, var->GetRootToken());
		op->SetOperand(0, new IRVariableOperand(var));
		op->SetOperand(1, new IRIntOperand(i));
	}

	mBody->GenCode(helper);
	helper.EmitLabel(mEndLabel, mRootToken);

	IRInstruction* popScope = helper.EmitInstruction(MoveStackPointer, mRootToken);
	popScope->SetOperand(0, new IRStackBytesOperand(true));

	helper.EmitReturn(mRootToken);
}
