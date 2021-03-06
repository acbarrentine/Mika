// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning(push)
#pragma warning(disable:4702) // unreachable code in the xtree standard header
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <memory>
#pragma warning(pop)

#if WIN32
#define NOMINMAX
#include <windows.h>

#define STRNCPY(dest, destSize, src, count) strncpy_s(dest, destSize, src, count)
#define SPRINTF(dest, ...) sprintf_s(dest, __VA_ARGS__)
#else
#define STRNCPY(dest, destSize, src, count) strncpy(dest, src, count)
#define SPRINTF(dest, ...) sprintf(dest, __VA_ARGS__)
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "MemoryManager.h"

#define ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

#if __cplusplus > 199711L
#define register      // Deprecated in C++11.
#endif  // #if __cplusplus > 199711L
