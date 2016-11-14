#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"

void Glue_Print(MikaVM* vm)
{
	MikaVM::Cell val = vm->GetFunctionArg(0);
	const char* str = (const char*)val.mPtrVal;
	MikaVM::Location loc = vm->GetLocation();

	char msgBuf[1024];
	sprintf_s(msgBuf, "%s:%i - %s\n", loc.Func->mName, loc.LineNumber, str);

	std::cout << msgBuf;	
	OutputDebugString(msgBuf);
}

void Glue_AssertEqualsInt(MikaVM* vm)
{
	MikaVM::Cell lhs = vm->GetFunctionArg(0);
	MikaVM::Cell rhs = vm->GetFunctionArg(1);
	assert (lhs.mIntVal == rhs.mIntVal);
}

void Glue_AssertEqualsFloat(MikaVM* vm)
{
	MikaVM::Cell lhs = vm->GetFunctionArg(0);
	MikaVM::Cell rhs = vm->GetFunctionArg(1);
	assert (lhs.mDblVal == rhs.mDblVal);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue("Print", Glue_Print);
	vm->RegisterGlue("AssertEqualsInt", Glue_AssertEqualsInt);
	vm->RegisterGlue("AssertEqualsFloat", Glue_AssertEqualsFloat);
}