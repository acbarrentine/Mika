#include "stdafx.h"
#include "Platform.h"
#include <libgen.h>
#include <sys/stat.h>
#include <string.h>

static char SPathBuffer[512];
const char* Platform::FileNameStem(const char* filePath)
{
    const char* dot = strrchr(filePath, '.');
    const char* slash = strrchr(filePath, '/');
    if (dot)
    {
        if (slash && dot > slash)
        {
            ptrdiff_t len = dot - slash - 1;
            strncpy(SPathBuffer, slash + 1, len);
            SPathBuffer[len] = '\0';
        }
        else
        {
            ptrdiff_t len = dot - filePath;
            strncpy(SPathBuffer, filePath, len);
            SPathBuffer[len] = '\0';
        }
    }
    else
    {
        strlcpy(SPathBuffer, filePath, sizeof(SPathBuffer));
    }
    return SPathBuffer;
}

void Platform::CreateDirectory(const char* filePath)
{
    dirname_r(filePath, SPathBuffer);
    mkdir(SPathBuffer, 0755);
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
