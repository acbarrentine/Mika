#include "stdafx.h"
#include "Platform.h"

static char SFileBuff[_MAX_FNAME];

const char* Platform::FileNameStem(const char* filePath)
{
    // split out the file name part of the incoming path
    _splitpath_s(filePath, nullptr, 0, nullptr, 0, SFileBuff, _MAX_FNAME, nullptr, 0);
    return SFileBuff;
}

void Platform::CreateDirectory(const char* filePath)
{
	char dir[_MAX_DIR];
	_splitpath_s(filePath, nullptr, 0, dir, _MAX_DIR, nullptr, 0, nullptr, 0);
	::CreateDirectory(dir, nullptr);
}

void Platform::Message(FILE* stream, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	Platform::MessageArgs(stream, format, args);
	va_end(args);
}

void Platform::MessageArgs(FILE* stream, const char* format, va_list args)
{
	char msgBuf[4096];
	vsprintf_s(msgBuf, format, args);
	fprintf(stream, msgBuf);
	OutputDebugStringA(msgBuf);

}
