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

void AssertEqualsFloat(float lhs, float rhs, const char* locationName, int lineNumber)
{
	CHECK_LINE(lhs == Approx(rhs), locationName, lineNumber);
}

void AssertEqualsString(const char* left, const char* right, const char* locationName, int lineNumber)
{
	CHECK_LINE(strcmp(left, right) == 0, locationName, lineNumber);
}

void AssertNotEqualsPointer(const char* left, const char* right, const char* locationName, int lineNumber)
{
	CHECK_LINE(left != right, locationName, lineNumber);
}

float Sqrt(float val)
{
	return sqrt(val);
}

const char* FindSubstring(const char* searchIn, const char* lookFor)
{
	return strstr(searchIn, lookFor);
}

struct Vec3
{
	float X;
	float Y;
	float Z;
	Vec3(float x, float y, float z)
		: X(x)
		, Y(y)
		, Z(z)
	{}

	float Magnitude() const
	{
		return sqrt(X*X + Y*Y + Z*Z);
	}

	void Normalize()
	{
		float mag = Magnitude();
		X /= mag;
		Y /= mag;
		Z /= mag;
	}

	float Dot(Vec3* other)
	{
		return X*other->X + Y*other->Y + Z*other->Z;
	}

	Vec3* Cross(Vec3* other)
	{
		float x = Y*other->Z - Z*other->Y;
		float y = Z*other->X - X*other->Z;
		float z = X*other->Y - Y*other->X;
		return new Vec3(x, y, z);
	}
};

#include "GeneratedGlue.hpp"
