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
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

#define ARRAY_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))
