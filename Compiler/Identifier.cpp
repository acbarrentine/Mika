#include "stdafx.h"
#include "Identifier.h"
#include <stdarg.h>

void StringTable::Reset()
{
	mStrings.clear();
}

Identifier StringTable::AddValue(std::string value)
{
	auto it = mStrings.insert(value);
	return Identifier(it.first->c_str());
}

Identifier StringTable::AddValue(const char* str)
{
	std::string val(str);
	return AddValue(val);
}

Identifier StringTable::AddValue(const char* first, const char* last)
{
	std::string val(first, last);
	return AddValue(val);
}

Identifier StringTable::ComposeValue(const char* pattern, ...)
{
	char buf[1024];
	va_list args;
	va_start(args, pattern);
	vsnprintf(buf, ARRAY_COUNT(buf), pattern, args);
	std::string val(buf);
	return AddValue(val);
}
