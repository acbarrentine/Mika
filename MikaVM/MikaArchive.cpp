#include "stdafx.h"
#include "MikaArchive.h"
#include "MikaScript.h"


template <typename T>
MikaReader& operator<<(MikaReader& ar, std::vector<T>& vec)
{
	size_t size;
	ar << size;
	vec.resize(size);
	ar.Serialize(&vec[0], size);
	return ar;
}

void MikaReader::Serialize(void* v, size_t size)
{
	mStream.read((char*)v, size);
}

void MikaReader::Process(const char* fileName, MikaScript* script)
{
	mStream.open(fileName, std::ios::in | std::ios::binary);

	if (mStream.good())
	{
		MikaArchiveFileHeader fileHeader;
		*this << fileHeader;

		if (fileHeader.mMagic != 'MIKA')
		{
			if (fileHeader.mMagic == 'AKIM')
			{
				// this would be where we handle byte-swapping
				// but... we're not doing that here today
			}
			else
			{
				std::cerr << "File '" << fileName << "' is not a Mika script opject file." << std::endl;
				return;
			}
		}

		*this << script->mStringData;
		*this << script->mByteData;

		for (unsigned int i = 0; i < fileHeader.mNumFunctions; ++i)
		{

		}
	}

}
