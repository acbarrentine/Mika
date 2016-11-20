#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"
#include "Catch.hpp"

void Print(const char* msg, const char* locationName, int lineNumber)
{
	char msgBuf[1024];
	sprintf_s(msgBuf, "%s:%i - %s\n", locationName, lineNumber, msg);

	std::cout << msgBuf;
	OutputDebugString(msgBuf);
}

void Assert(int cond, const char* locationName, int lineNumber)
{
	CHECK_LINE(cond, locationName, lineNumber);
}

void AssertEqualsInt(int lhs, int rhs, const char* locationName, int lineNumber)
{
	CHECK_LINE(lhs == rhs, locationName, lineNumber);
}

void AssertEqualsFloat(double lhs, double rhs, const char* locationName, int lineNumber)
{
	CHECK_LINE(lhs == rhs, locationName, lineNumber);
}

#include "GeneratedGlue.hpp"
