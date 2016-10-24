#include "stdafx.h"
#include "MikaArchive.h"


void MikaReader::Open(const char* fileName)
{
	mStream.open(fileName, std::ios::in | std::ios::binary);
	mFailed = !mStream.good();
}
