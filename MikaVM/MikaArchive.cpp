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

		// allocate the global context for use with this script's functions
		MikaVM::GlobalContext* globalContext = new MikaVM::GlobalContext;
		globalContext->mStringData = std::move(fileHeader.mStringData);
		const char* globalName = nullptr;

		// add each function to the VM
		for (size_t i = 0; i < fileHeader.mFunctions.size(); ++i)
		{
			MikaArchiveFunctionHeader& arHeader = fileHeader.mFunctions[i];
			
			MikaVM::Function runTimeHeader;
			runTimeHeader.mByteData = std::move(arHeader.mByteData);
			runTimeHeader.mName = &globalContext->mStringData[arHeader.mNameOffset];
			runTimeHeader.mStackSize = arHeader.mStackSize;
			runTimeHeader.mGlobalContext = globalContext;

			if (i == 0)
			{
				globalName = runTimeHeader.mName;
			}

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

			// fixup strings
			for (int& fixup : arHeader.mStringFixups)
			{
				MikaVM::Cell* location = (MikaVM::Cell*) (&runTimeHeader.mByteData[fixup]);
				const char* str = &globalContext->mStringData[location->mIntVal];
				location->mPtrVal = (void*) str;
			}

			vm->mScriptFunctions.insert(std::make_pair(runTimeHeader.mName, std::move(runTimeHeader)));
		}

		// initialize the global stack
		if (globalName)
		{
			globalContext->mStack.insert(globalContext->mStack.begin(), fileHeader.mGlobalStackSize, 0);
			vm->Execute(globalName);
		}
	}
}
