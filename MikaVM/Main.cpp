#include "stdafx.h"
#include "MikaVM.h"
#include <direct.h>


int main()
{
	//char cwdBuf[1024];
	//_getcwd(cwdBuf, sizeof(cwdBuf));

	MikaScript script("..\\TestScripts\\FirstRun.miko");
	MikaVM vm;
	vm.Import(script);
	vm.Execute("FirstRun:TestEntry");
	return 0;
}

