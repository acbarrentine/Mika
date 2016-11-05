#include "stdafx.h"
#include "Identifier.h"
#include "ScriptFunction.h"
#include "FunctionDeclaration.h"
#include "Statement.h"
#include "Variable.h"
#include "SymbolTable.h"
#include "ObjectFileHelper.h"
#include "IRCode.h"


Type* ScriptFunction::GetReturnType() const
{
	return mDeclaration->GetReturnType();
}

void ScriptFunction::ResolveTypes(SymbolTable& symbolTable)
{
	symbolTable.Push();
	mDeclaration->BindParameters(symbolTable);
	mStatement->ResolveTypes(symbolTable);
	symbolTable.Pop();
}

void ScriptFunction::GenCode(ObjectFileHelper& helper)
{
	// save called arguments
	for (size_t i = 0; i < mDeclaration->GetParameterCount(); ++i)
	{
		Variable* var = mDeclaration->GetParameter(i);
		IRInstruction& op = helper.EmitInstruction(CopyArgToStack, var->GetRootToken());
		op.SetOperand(0, new IRVariableOperand(var));
		op.SetOperand(1, new IRIntOperand(i));
	}

	mStatement->GenCode(helper);
	helper.EmitInstruction(IllegalInstruction, mRootToken);
}
