#pragma once

void Glue_Print(MikaVM* vm)
{
	MikaVM::Location& loc = vm->GetLocation();
	const char* param1 = (const char*)vm->GetFunctionArg(0).mStrVal;
	Print(param1, loc.Func->mName, loc.LineNumber);
}

void Glue_DeleteV3(void* addr)
{
	V3* obj = (V3*)addr;
	delete obj;
}

void Glue_V3_Empty(MikaVM* vm)
{
	V3* retval = new V3();
	vm->SetResultRegister(retval);
	vm->RegisterDestructor(retval, Glue_DeleteV3);
}

void Glue_V3_XYZ(MikaVM* vm)
{
	float x = (float)vm->GetFunctionArg(0).mFltVal;
	float y = (float)vm->GetFunctionArg(1).mFltVal;
	float z = (float)vm->GetFunctionArg(2).mFltVal;
	V3* retval = new V3(x, y, z);
	vm->SetResultRegister(retval);
	vm->RegisterDestructor(retval, Glue_DeleteV3);
}

void Glue_V3_Copy(MikaVM* vm)
{
	V3* other = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* retval = new V3(other);
	vm->SetResultRegister(retval);
	vm->RegisterDestructor(retval, Glue_DeleteV3);
}

void Glue_V3_Assign(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	obj->Assign(other);
}

void Glue_V3_Add(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	obj->Add(other);
}

void Glue_V3_Subtract(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	obj->Subtract(other);
}

void Glue_V3_Multiply(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	obj->Multiply(other);
}

void Glue_V3_Divide(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	obj->Divide(other);
}

void Glue_V3_Scale(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	float scalar = (float)vm->GetFunctionArg(1).mFltVal;
	obj->Scale(scalar);
}

void Glue_V3_Equals(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	int retval = obj->Equals(other);
	vm->SetResultRegister(retval);
}

void Glue_V3_NotEquals(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	int retval = obj->NotEquals(other);
	vm->SetResultRegister(retval);
}

void Glue_V3_GetLength(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	float retval = obj->GetLength();
	vm->SetResultRegister(retval);
}

void Glue_V3_GetLengthSquared(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	float retval = obj->GetLengthSquared();
	vm->SetResultRegister(retval);
}

void Glue_V3_DotProduct(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* vec2 = (V3*)vm->GetFunctionArg(1).mPtrVal;
	float retval = obj->DotProduct(vec2);
	vm->SetResultRegister(retval);
}

void Glue_V3_CrossProduct(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* vec2 = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3* retval = obj->CrossProduct(vec2);
	vm->SetResultRegister(retval);
	vm->RegisterDestructor(retval, Glue_DeleteV3);
}

void Glue_V3_Normalize(MikaVM* vm)
{
	V3* obj = (V3*)vm->GetFunctionArg(0).mPtrVal;
	obj->Normalize();
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue({
		{ "Print", Glue_Print },
		{ "V3_Empty", Glue_V3_Empty },
		{ "V3_XYZ", Glue_V3_XYZ },
		{ "V3_Copy", Glue_V3_Copy },
		{ "V3_Assign", Glue_V3_Assign },
		{ "V3_Add", Glue_V3_Add },
		{ "V3_Subtract", Glue_V3_Subtract },
		{ "V3_Multiply", Glue_V3_Multiply },
		{ "V3_Divide", Glue_V3_Divide },
		{ "V3_Scale", Glue_V3_Scale },
		{ "V3_Equals", Glue_V3_Equals },
		{ "V3_NotEquals", Glue_V3_NotEquals },
		{ "V3_GetLength", Glue_V3_GetLength },
		{ "V3_GetLengthSquared", Glue_V3_GetLengthSquared },
		{ "V3_DotProduct", Glue_V3_DotProduct },
		{ "V3_CrossProduct", Glue_V3_CrossProduct },
		{ "V3_Normalize", Glue_V3_Normalize },
	});
}
