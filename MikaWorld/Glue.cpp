#include "stdafx.h"
#include "Glue.h"
#include "MikaVM.h"

typedef irr::core::vector3df V3;

V3* V3_ConstructEmpty()
{
	return new V3();
}

V3* V3_ConstructWithValues(float X, float Y, float Z)
{
	return new V3((float)X, (float)Y, (float)Z);
}

V3* V3_ConstructCopy(V3* other)
{
	return new V3(*other);
}

void V3_Assign(V3* target, V3* other)
{
	*target = *other;
}

void V3_Add(V3* target, V3* other)
{
	*target += *other;
}

void V3_Subtract(V3* target, V3* other)
{
	*target -= *other;
}

void V3_Multiply(V3* target, V3* other)
{
	*target *= *other;
}

void V3_Divide(V3* target, V3* other)
{
	*target /= *other;
}

void V3_Scale(V3* target, float by)
{
	*target *= (float)by;
}

int V3_Equals(V3* left, V3* right)
{
	return *left == *right;
}

int V3_NotEquals(V3* left, V3* right)
{
	return *left != *right;
}

float V3_GetLength(V3* vec)
{
	return vec->getLength();
}

float V3_GetLengthSquared(V3* vec)
{
	return vec->getLengthSQ();
}

float V3_DotProduct(V3* source, V3* target)
{
	return source->dotProduct(*target);
}

V3* V3_CrossProduct(V3* source, V3* target)
{
	V3 temp = source->crossProduct(*target);
	return new V3(temp);
}

void V3_Normalize(V3* vec)
{
	vec->normalize();
}

void V3_Delete(V3* vec)
{
	delete vec;
}

void Print(const char* msg)
{
#if WIN32
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
#else
    printf("%s\n", msg);
#endif
}

#include "GeneratedGlue.hpp"
