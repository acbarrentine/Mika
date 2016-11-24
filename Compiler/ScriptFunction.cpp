#include "stdafx.h"
#include "Identifier.h"
#include "ScriptFunction.h"
#include "FunctionDeclaration.h"
#include "Statement.h"
#include "Variable.h"
#include "SymbolTable.h"
#include "ObjectFileHelper.h"
#include "IRCode.h"
#include "Token.h"
#include "Compiler.h"

void ScriptFunction::SetName(Identifier name)
{
	Token& tok = GCompiler.GetToken(mRootToken);
	Identifier fileNameStem = GCompiler.GetStemName(tok.GetFileIndex());
	mName = GCompiler.ComposeIdentifier("%s:%s", fileNameStem.GetString(), name.GetString());
}

void ScriptFunction::ResolveTypes(SymbolTable& symbolTable)
{
	if (!mIsGlobal)
		symbolTable.Push();
	
	BindParameters(symbolTable);
	mStatement->ResolveTypes(symbolTable);
	
	if (!mIsGlobal) symbolTable.Pop();
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
	helper.EmitReturn(mRootToken);
}
