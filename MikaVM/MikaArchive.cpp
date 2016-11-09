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

			for (int& fixup : arHeader.mStringFixups)
			{
				MikaVM::Cell* location = (MikaVM::Cell*) (&runTimeHeader.mByteData[fixup]);
				const char* str = &runTimeHeader.mStringData[location->mIntVal];
				location->mPtrVal = (void*) str;
			}

			vm->mScriptFunctions.insert(std::make_pair(runTimeHeader.mName, std::move(runTimeHeader)));
		}
	}
}
