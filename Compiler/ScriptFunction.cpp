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
	mStatement->AddStatement(stmt);
}

void ScriptFunction::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.StartFunction();
	symbolTable.Push();

	mEndLabel = symbolTable.GenLabel(GCompiler.AddIdentifier("end"), symbolTable.GetNextLabelSequence());

	BindParameters(symbolTable);
	mStatement->ResolveTypes(symbolTable);
	
	symbolTable.Pop();
}

void ScriptFunction::GenCode(ObjectFileHelper& helper)
{
	// save called arguments
	for (int i = 0; i < GetParameterCount(); ++i)
	{
		Variable* var = GetParameter(i);
		IRInstruction* op = helper.EmitInstruction(CopyArgToStack, var->GetRootToken());
		op->SetOperand(0, new IRVariableOperand(var));
		op->SetOperand(1, new IRIntOperand(i));
	}

	mStatement->GenCode(helper);
	helper.EmitLabel(mEndLabel, mRootToken);
	helper.EmitReturn(mRootToken);
}
