#include "stdafx.h"
#include "MikaVM.h"

int main()
{
	MikaScript script("FirstRun.miko");
	MikaVM vm;
	vm.Import(script);
	vm.Execute("FirstRun:TestEntry");
	return 0;
}

