#pragma once

void Glue_Print(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	const char* param1 = (const char*)vm->GetFunctionArg(0).mStrVal;
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
	const char* left = (const char*)vm->GetFunctionArg(0).mStrVal;
	const char* right = (const char*)vm->GetFunctionArg(1).mStrVal;
	AssertEqualsString(left, right, loc.Func->mName, loc.LineNumber);
}

void Glue_AssertNotEqualsPointer(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	const char* left = (const char*)vm->GetFunctionArg(0).mStrVal;
	const char* right = (const char*)vm->GetFunctionArg(1).mStrVal;
	AssertNotEqualsPointer(left, right, loc.Func->mName, loc.LineNumber);
}

void Glue_Sqrt(MikaVM* vm)
{
	double param1 = (double)vm->GetFunctionArg(0).mDblVal;
	double retval = Sqrt(param1);
	vm->SetResultRegister(retval);
}

void Glue_FindSubstring(MikaVM* vm)
{
	const char* param1 = (const char*)vm->GetFunctionArg(0).mStrVal;
	const char* param2 = (const char*)vm->GetFunctionArg(1).mStrVal;
	const char* retval = FindSubstring(param1, param2);
	vm->SetResultRegister(retval);
}

void Glue_CreateVec3(MikaVM* vm)
{
	double X = (double)vm->GetFunctionArg(0).mDblVal;
	double Y = (double)vm->GetFunctionArg(1).mDblVal;
	double Z = (double)vm->GetFunctionArg(2).mDblVal;
	Vec3* retval = CreateVec3(X, Y, Z);
	vm->SetResultRegister(retval);
}

void Glue_DeleteVec3(MikaVM* vm)
{
	Vec3* param1 = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	DeleteVec3(param1);
}

void Glue_Normalize(MikaVM* vm)
{
	Vec3* param1 = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	Normalize(param1);
}

void Glue_Magnitude(MikaVM* vm)
{
	Vec3* param1 = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	double retval = Magnitude(param1);
	vm->SetResultRegister(retval);
}

void Glue_Dot(MikaVM* vm)
{
	Vec3* vec1 = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	Vec3* vec2 = (Vec3*)vm->GetFunctionArg(1).mPtrVal;
	double retval = Dot(vec1, vec2);
	vm->SetResultRegister(retval);
}

void Glue_Cross(MikaVM* vm)
{
	Vec3* vec1 = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	Vec3* vec2 = (Vec3*)vm->GetFunctionArg(1).mPtrVal;
	Vec3* outVec = (Vec3*)vm->GetFunctionArg(2).mPtrVal;
	Cross(vec1, vec2, outVec);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue({
		{ "Print", Glue_Print },
		{ "Assert", Glue_Assert },
		{ "AssertEqualsInt", Glue_AssertEqualsInt },
		{ "AssertEqualsFloat", Glue_AssertEqualsFloat },
		{ "AssertEqualsString", Glue_AssertEqualsString },
		{ "AssertNotEqualsPointer", Glue_AssertNotEqualsPointer },
		{ "Sqrt", Glue_Sqrt },
		{ "FindSubstring", Glue_FindSubstring },
		{ "CreateVec3", Glue_CreateVec3 },
		{ "DeleteVec3", Glue_DeleteVec3 },
		{ "Normalize", Glue_Normalize },
		{ "Magnitude", Glue_Magnitude },
		{ "Dot", Glue_Dot },
		{ "Cross", Glue_Cross },
	});
}
