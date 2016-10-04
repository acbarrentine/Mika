#pragma once

#include "Identifier.h"
#include "Token.h"

enum class MsgSeverity : int
{
	kInfo,
	kWarn,
	kError,
	kError2,
};

class Compiler
{
protected:
	int mErrorCount;
	std::vector<Token> mTokenList;
	std::vector<std::string> mFileNames;
	StringTable mIdentifiers;
	const int kInitialTokenCount = 200000;

public:
	Compiler();

	void Message(MsgSeverity severity, const char* format, ...);
	void MessageArgs(MsgSeverity severity, const char* format, va_list args);

	void ReadGlue(const char* fileName);
	void ReadScript(const char* fileName);

	const char* GetFileName(int fileIndex) const;
	int GetErrorCount() const { return mErrorCount; }
	Identifier AddIdentifier(const char* first, const char* last);
	Token& CreateToken(TType tokenType, int fileIndex, int lineNumber, const char* str, int len);
};

extern Compiler GCompiler;
