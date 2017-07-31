#pragma once

void Glue_V3_ConstructEmpty(MikaVM* vm)
{
	V3* retval = V3_ConstructEmpty();
	vm->SetResultRegister(retval);
}

void Glue_V3_ConstructWithValues(MikaVM* vm)
{
	float X = (float)vm->GetFunctionArg(0).mFltVal;
	float Y = (float)vm->GetFunctionArg(1).mFltVal;
	float Z = (float)vm->GetFunctionArg(2).mFltVal;
	V3* retval = V3_ConstructWithValues(X, Y, Z);
	vm->SetResultRegister(retval);
}

void Glue_V3_ConstructCopy(MikaVM* vm)
{
	V3* other = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* retval = V3_ConstructCopy(other);
	vm->SetResultRegister(retval);
}

void Glue_V3_Assign(MikaVM* vm)
{
	V3* target = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3_Assign(target, other);
}

void Glue_V3_Add(MikaVM* vm)
{
	V3* target = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3_Add(target, other);
}

void Glue_V3_Subtract(MikaVM* vm)
{
	V3* target = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3_Subtract(target, other);
}

void Glue_V3_Multiply(MikaVM* vm)
{
	V3* target = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3_Multiply(target, other);
}

void Glue_V3_Divide(MikaVM* vm)
{
	V3* target = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* other = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3_Divide(target, other);
}

void Glue_V3_Scale(MikaVM* vm)
{
	V3* target = (V3*)vm->GetFunctionArg(0).mPtrVal;
	float by = (float)vm->GetFunctionArg(1).mFltVal;
	V3_Scale(target, by);
}

void Glue_V3_Equals(MikaVM* vm)
{
	V3* left = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* right = (V3*)vm->GetFunctionArg(1).mPtrVal;
	int retval = V3_Equals(left, right);
	vm->SetResultRegister(retval);
}

void Glue_V3_NotEquals(MikaVM* vm)
{
	V3* left = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* right = (V3*)vm->GetFunctionArg(1).mPtrVal;
	int retval = V3_NotEquals(left, right);
	vm->SetResultRegister(retval);
}

void Glue_V3_GetLength(MikaVM* vm)
{
	V3* param1 = (V3*)vm->GetFunctionArg(0).mPtrVal;
	float retval = V3_GetLength(param1);
	vm->SetResultRegister(retval);
}

void Glue_V3_GetLengthSquared(MikaVM* vm)
{
	V3* param1 = (V3*)vm->GetFunctionArg(0).mPtrVal;
	float retval = V3_GetLengthSquared(param1);
	vm->SetResultRegister(retval);
}

void Glue_V3_DotProduct(MikaVM* vm)
{
	V3* source = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* target = (V3*)vm->GetFunctionArg(1).mPtrVal;
	float retval = V3_DotProduct(source, target);
	vm->SetResultRegister(retval);
}

void Glue_V3_CrossProduct(MikaVM* vm)
{
	V3* source = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3* target = (V3*)vm->GetFunctionArg(1).mPtrVal;
	V3* retval = V3_CrossProduct(source, target);
	vm->SetResultRegister(retval);
}

void Glue_V3_Normalize(MikaVM* vm)
{
	V3* param1 = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3_Normalize(param1);
}

void Glue_V3_Delete(MikaVM* vm)
{
	V3* param1 = (V3*)vm->GetFunctionArg(0).mPtrVal;
	V3_Delete(param1);
}

void Glue_Print(MikaVM* vm)
{
	const char* param1 = (const char*)vm->GetFunctionArg(0).mStrVal;
	Print(param1);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue({
		{ "V3_ConstructEmpty", Glue_V3_ConstructEmpty },
		{ "V3_ConstructWithValues", Glue_V3_ConstructWithValues },
		{ "V3_ConstructCopy", Glue_V3_ConstructCopy },
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
		{ "V3_Delete", Glue_V3_Delete },
		{ "Print", Glue_Print },
	});
}
