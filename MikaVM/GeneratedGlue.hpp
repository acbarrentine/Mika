#pragma once

void Glue_Print(MikaVM* vm)
{
	const char* param1 = (const char*)vm->GetFunctionArg(0).mPtrVal;
	MikaVM::Location& loc = vm->GetLocation();
	Print(param1, loc.Func->mName, loc.LineNumber);
}

void Glue_Assert(MikaVM* vm)
{
	int cond = (int)vm->GetFunctionArg(0).mIntVal;
	MikaVM::Location& loc = vm->GetLocation();
	Assert(cond, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsInt(MikaVM* vm)
{
	int left = (int)vm->GetFunctionArg(0).mIntVal;
	int right = (int)vm->GetFunctionArg(1).mIntVal;
	MikaVM::Location& loc = vm->GetLocation();
	AssertEqualsInt(left, right, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsFloat(MikaVM* vm)
{
	double left = (double)vm->GetFunctionArg(0).mDblVal;
	double right = (double)vm->GetFunctionArg(1).mDblVal;
	MikaVM::Location& loc = vm->GetLocation();
	AssertEqualsFloat(left, right, loc.Func->mName, loc.LineNumber);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue({
		{ "Print", Glue_Print },
		{ "Assert", Glue_Assert },
		{ "AssertEqualsInt", Glue_AssertEqualsInt },
		{ "AssertEqualsFloat", Glue_AssertEqualsFloat },
	});
}
