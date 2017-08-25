#include "stdafx.h"
#include "Glue.h"
#include "../MikaVM/MikaVM.h"
#include "MikaWorld.h"

void Print(const char* msg, const char* locationName, int lineNumber)
{
	char buf[2048];
	sprintf_s(buf, sizeof(buf), "%s:%i - %s\n", locationName, lineNumber, msg);
	GWorld.AddScriptMessage(msg);
}

#include "GeneratedGlue.hpp"
