#pragma once

void Glue_Print(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	MikaString* param1 = (MikaString*)vm->GetFunctionArg(0).mStrVal;
	Print(param1, loc.Func->mName, loc.LineNumber);
}

void Glue_Assert(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	int cond = (int)vm->GetFunctionArg(0).mIntVal;
	Assert(cond, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsInt(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	int left = (int)vm->GetFunctionArg(0).mIntVal;
	int right = (int)vm->GetFunctionArg(1).mIntVal;
	AssertEqualsInt(left, right, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsFloat(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	double left = (double)vm->GetFunctionArg(0).mDblVal;
	double right = (double)vm->GetFunctionArg(1).mDblVal;
	AssertEqualsFloat(left, right, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertEqualsString(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	MikaString* left = (MikaString*)vm->GetFunctionArg(0).mStrVal;
	MikaString* right = (MikaString*)vm->GetFunctionArg(1).mStrVal;
	AssertEqualsString(left, right, loc.Func->mName, loc.LineNumber);
}

void Glue_Sqrt(MikaVM* vm)
{
	double param1 = (double)vm->GetFunctionArg(0).mDblVal;
	double retval = Sqrt(param1);
	vm->SetResultRegister(retval);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue({
		{ "Print", Glue_Print },
		{ "Assert", Glue_Assert },
		{ "AssertEqualsInt", Glue_AssertEqualsInt },
		{ "AssertEqualsFloat", Glue_AssertEqualsFloat },
		{ "AssertEqualsString", Glue_AssertEqualsString },
		{ "Sqrt", Glue_Sqrt },
	});
}
