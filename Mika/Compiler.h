#pragma once

#include "Tokenizer.h"

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
	Tokenizer* mTokenizer;
	
	const int kInitialTokenCount = 200000;

public:
	Compiler();

	void Message(MsgSeverity severity, const char* format, ...);
	void MessageArgs(MsgSeverity severity, const char* format, va_list args);

	void ReadScript(const char* fileName);

	int GetErrorCount() const { return mErrorCount; }
	Token& CreateToken(TType tokenType, int lineNumber);
};

extern Compiler GCompiler;
