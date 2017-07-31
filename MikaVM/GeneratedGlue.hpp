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
	float left = (float)vm->GetFunctionArg(0).mFltVal;
	float right = (float)vm->GetFunctionArg(1).mFltVal;
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
	float param1 = (float)vm->GetFunctionArg(0).mFltVal;
	float retval = Sqrt(param1);
	vm->SetResultRegister(retval);
}

void Glue_FindSubstring(MikaVM* vm)
{
	const char* param1 = (const char*)vm->GetFunctionArg(0).mStrVal;
	const char* param2 = (const char*)vm->GetFunctionArg(1).mStrVal;
	const char* retval = FindSubstring(param1, param2);
	vm->SetResultRegister(retval);
}

void Glue_DeleteVec3(void* addr)
{
	Vec3* obj = (Vec3*)addr;
	delete obj;
}

void Glue_Vec3_XYZ(MikaVM* vm)
{
	float X = (float)vm->GetFunctionArg(0).mFltVal;
	float Y = (float)vm->GetFunctionArg(1).mFltVal;
	float Z = (float)vm->GetFunctionArg(2).mFltVal;
	Vec3* retval = new Vec3(X, Y, Z);
	vm->SetResultRegister(retval);
	vm->RegisterDestructor(retval, Glue_DeleteVec3);
}

void Glue_Vec3_Normalize(MikaVM* vm)
{
	Vec3* obj = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	obj->Normalize();
}

void Glue_Vec3_Magnitude(MikaVM* vm)
{
	Vec3* obj = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	float retval = obj->Magnitude();
	vm->SetResultRegister(retval);
}

void Glue_Vec3_Dot(MikaVM* vm)
{
	Vec3* obj = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	Vec3* vec2 = (Vec3*)vm->GetFunctionArg(1).mPtrVal;
	float retval = obj->Dot(vec2);
	vm->SetResultRegister(retval);
}

void Glue_Vec3_Cross(MikaVM* vm)
{
	Vec3* obj = (Vec3*)vm->GetFunctionArg(0).mPtrVal;
	Vec3* vec2 = (Vec3*)vm->GetFunctionArg(1).mPtrVal;
	Vec3* retval = obj->Cross(vec2);
	vm->SetResultRegister(retval);
	vm->RegisterDestructor(retval, Glue_DeleteVec3);
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
		{ "Vec3_XYZ", Glue_Vec3_XYZ },
		{ "Vec3_Normalize", Glue_Vec3_Normalize },
		{ "Vec3_Magnitude", Glue_Vec3_Magnitude },
		{ "Vec3_Dot", Glue_Vec3_Dot },
		{ "Vec3_Cross", Glue_Vec3_Cross },
	});
}
