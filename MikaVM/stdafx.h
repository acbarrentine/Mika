// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdint.h>
#include <vector>
#include <set>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <map>
#include <cassert>
#include <memory>

#if WIN32
#define NOMINMAX
#include <windows.h>

#define STRNCPY(dest, destSize, src, count) strncpy_s(dest, destSize, src, count)
#define SPRINTF(dest, ...) sprintf_s(dest, __VA_ARGS__)
#else
#define STRNCPY(dest, destSize, src, count) strncpy(dest, src, count)
#define SPRINTF(dest, ...) sprintf(dest, __VA_ARGS__)
#endif

#define ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))
