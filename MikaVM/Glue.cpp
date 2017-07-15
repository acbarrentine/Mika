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

struct Vec3
{
	double X;
	double Y;
	double Z;
	Vec3(double x, double y, double z)
		: X(x)
		, Y(y)
		, Z(z)
	{}
};

Vec3* CreateVec3(double x, double y, double z)
{
	return new Vec3(x, y, z);
}

void DeleteVec3(Vec3* vec)
{
	delete vec;
}

double Magnitude(Vec3* vec)
{
	return sqrt(vec->X*vec->X + vec->Y*vec->Y + vec->Z*vec->Z);
}

void Normalize(Vec3* vec)
{
	double mag = Magnitude(vec);
	vec->X /= mag;
	vec->Y /= mag;
	vec->Z /= mag;
}

double Dot(Vec3* vec1, Vec3* vec2)
{
	return vec1->X*vec2->X + vec1->Y*vec2->Y + vec1->Z*vec2->Z;
}

void Cross(Vec3* vec1, Vec3* vec2, Vec3* outVec)
{
	outVec->X = vec1->Y*vec2->Z - vec1->Z*vec2->Y;
	outVec->Y = vec1->Z*vec2->X - vec1->X*vec2->Z;
	outVec->Z = vec1->X*vec2->Y - vec1->Y*vec2->X;
}

#include "GeneratedGlue.hpp"
