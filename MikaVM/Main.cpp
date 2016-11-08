#include "stdafx.h"
#include "MikaVM.h"
#include <direct.h>


int main()
{
	//char cwdBuf[1024];
	//_getcwd(cwdBuf, sizeof(cwdBuf));

	MikaVM vm;
	vm.Import("..\\TestScripts\\FirstRun.miko");
	vm.Execute("FirstRun:TestEntry");
	return 0;
}

