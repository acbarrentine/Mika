#include "stdafx.h"
#include "MikaArchive.h"
#include "MikaVM.h"

void MikaReader::Serialize(void* v, int size)
{
	mStream.read((char*)v, size);
}

void MikaReader::Process(const char* fileName, MikaVM* vm)
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
				std::cerr << "File '" << fileName << "' is not a Mika script object file." << std::endl;
				return;
			}
		}

		// split out the file name part of the incoming path
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath_s(fileName, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);

		for (MikaArchiveFunctionHeader& arHeader : fileHeader.mFunctions)
		{
			MikaVM::Function runTimeHeader;
			runTimeHeader.mStringData = std::move(arHeader.mStringData);
			runTimeHeader.mByteData = std::move(arHeader.mByteData);
			runTimeHeader.mName = &runTimeHeader.mStringData[0];
			runTimeHeader.mStackSize = arHeader.mStackSize;

			// fixup function references
			for (size_t pcOffset = 0; pcOffset < runTimeHeader.mByteData.size(); )
			{
				MikaVM::Instruction* op = (MikaVM::Instruction*)&runTimeHeader.mByteData[pcOffset];
				MikaArchiveInstruction* archiveVersion = (MikaArchiveInstruction*)op;
				if (op->mFunc)
				{
					op->mFunc = MikaVM::SBuiltInFunctions[archiveVersion->mCode];
				}
				pcOffset += op->GetSize();
			}

			std::string key = fname;
			key += ":";
			key += runTimeHeader.mName;
			vm->mFunctions.insert(std::make_pair(key, std::move(runTimeHeader)));
		}
	}
}
