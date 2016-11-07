#include "stdafx.h"
#include "MikaArchive.h"
#include "MikaScript.h"

void MikaReader::Serialize(void* v, int size)
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
				std::cerr << "File '" << fileName << "' is not a Mika script object file." << std::endl;
				return;
			}
		}

		script->mByteData = std::move(fileHeader.mByteData);
		script->mStringData = std::move(fileHeader.mStringData);

		script->mFunctions.reserve(fileHeader.mFunctions.size());
		for (MikaArchiveFunctionHeader& arHeader : fileHeader.mFunctions)
		{
			MikaScript::FunctionHeader runTimeHeader;
			runTimeHeader.mName = &script->mStringData[arHeader.mNameOffset];
			runTimeHeader.mByteCodeOffset = arHeader.mByteCodeOffset;
			runTimeHeader.mStackSize = arHeader.mStackSize;
			script->mFunctions.push_back(runTimeHeader);

			std::cout << "Adding func " << runTimeHeader.mName << " to script." << std::endl;
		}

		// fixup function references
		for (size_t pcOffset = 0; pcOffset < script->mByteData.size(); )
		{
			MikaScript::Instruction* op = (MikaScript::Instruction*) &script->mByteData[pcOffset];
			MikaArchiveInstruction* archiveVersion = (MikaArchiveInstruction*)op;
			if (op->mFunc)
			{
				op->mFunc = MikaScript::SBuiltInFunctions[archiveVersion->mCode];
			}
			pcOffset += op->GetSize();
		}
	}
}
