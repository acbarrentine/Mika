#include "stdafx.h"
#include "Platform.h"

static char SFileBuff[_MAX_FNAME];

const char* Platform::FileNameStem(const char* filePath)
{
    // split out the file name part of the incoming path
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char ext[_MAX_EXT];
    _splitpath_s(filePath, drive, _MAX_DRIVE, dir, _MAX_DIR, SFileBuff, _MAX_FNAME, ext, _MAX_EXT);
    return SFileBuff;
}

void Platform::CreateDirectory(const char* filePath)
{
	::CreateDirectory(filePath, nullptr);
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
	vfprintf(stream, format, args);
}
