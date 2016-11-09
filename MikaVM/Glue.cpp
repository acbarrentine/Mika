#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"

void Glue_Print(MikaVM* vm)
{
	MikaVM::Cell val = vm->GetFunctionArg(0);
	const char* str = (const char*)val.mPtrVal;
	std::cout << str << std::endl;
	
	OutputDebugString(str);
	OutputDebugString("\n");
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue("Print", Glue_Print);
}