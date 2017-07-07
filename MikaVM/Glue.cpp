#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"
#include "Catch.hpp"
#include "../Compiler/Platform.h"
#include <math.h>

void Print(const char* msg, const char* locationName, int lineNumber)
{
	char msgBuf[1024];
	SPRINTF(msgBuf, "%s:%i - %s\n", locationName, lineNumber, msg);

	Platform::Message(stdout, msgBuf);
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

void AssertEqualsString(const char* left, const char* right, const char* locationName, int lineNumber)
{
	CHECK_LINE(strcmp(left, right) == 0, locationName, lineNumber);
}

void AssertNotEqualsPointer(const char* left, const char* right, const char* locationName, int lineNumber)
{
	CHECK_LINE(left != right, locationName, lineNumber);
}

double Sqrt(double val)
{
	return sqrt(val);
}

const char* FindSubstring(const char* searchIn, const char* lookFor)
{
	return strstr(searchIn, lookFor);
}

#include "GeneratedGlue.hpp"
