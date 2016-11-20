#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"
#include "Catch.hpp"

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

void Glue_Assert(MikaVM* vm)
{
	MikaVM::Cell cond = vm->GetFunctionArg(0);
	MikaVM::Location& loc = vm->GetLocation();
	CHECK_LINE(cond.mIntVal, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsInt(MikaVM* vm)
{
	MikaVM::Cell lhs = vm->GetFunctionArg(0);
	MikaVM::Cell rhs = vm->GetFunctionArg(1);
	MikaVM::Location& loc = vm->GetLocation();
	CHECK_LINE(lhs.mIntVal == rhs.mIntVal, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsFloat(MikaVM* vm)
{
	MikaVM::Cell lhs = vm->GetFunctionArg(0);
	MikaVM::Cell rhs = vm->GetFunctionArg(1);
	MikaVM::Location& loc = vm->GetLocation();
	CHECK_LINE(lhs.mDblVal == rhs.mDblVal, loc.Func->mName, loc.LineNumber);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue("Print", Glue_Print);
	vm->RegisterGlue("Assert", Glue_Assert);
	vm->RegisterGlue("AssertEqualsInt", Glue_AssertEqualsInt);
	vm->RegisterGlue("AssertEqualsFloat", Glue_AssertEqualsFloat);
}