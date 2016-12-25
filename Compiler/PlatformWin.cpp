#include "stdafx.h"
#include "Platform.h"

static SFileBuff[_MAX_FNAME];

const char* Platform::FileNameStem(const char* filePath)
{
    // split out the file name part of the incoming path
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath_s(fileName, drive, _MAX_DRIVE, dir, _MAX_DIR, SFileBuff, _MAX_FNAME, ext, _MAX_EXT);
    return SFileBuff;
}

void Platform::CreateDirectory(const char* filePath)
{
    dirname_r(filePath, SPathBuffer);
    mkdir(SPathBuffer, 0755);
}
