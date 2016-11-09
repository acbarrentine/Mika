#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"

void Glue_Print(MikaVM* vm)
{
	MikaVM::Cell val = vm->GetFunctionArg(0);
	const char* str = (const char*)val.mPtrVal;
	MikaVM::Location loc = vm->GetLocation();

	char msgBuf[1024];
	sprintf_s(msgBuf, "%s:%i - %s\n", loc.Function, loc.LineNumber, str);

	std::cout << msgBuf;	
	OutputDebugString(msgBuf);
}

void RegisterGlueFunctions(MikaVM* vm)
{
	vm->RegisterGlue("Print", Glue_Print);
}