#include "stdafx.h"
#include "Compiler.h"
#include <stdarg.h>

Compiler GCompiler;

Compiler::Compiler()
	: mErrorCount(0)
{
	mTokenList.reserve(kInitialTokenCount);
}

void Compiler::Message(MsgSeverity severity, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	MessageArgs(severity, format, args);
	va_end(args);
}

static FILE* GetStream(MsgSeverity severity)
{
	switch (severity)
	{
		case MsgSeverity::kError:
		case MsgSeverity::kError2:
		case MsgSeverity::kWarn:
			return stderr;

		default:
		case MsgSeverity::kInfo:
			return stdout;
	}
}

void Compiler::MessageArgs(MsgSeverity severity, const char* format, va_list args)
{
	FILE* stream = GetStream(severity);
	static char buf[64 * 1024];

	int len = vsnprintf(buf, sizeof(buf), format, args);

	OutputDebugString(buf);

	if (severity == MsgSeverity::kError)
		++mErrorCount;

	static HANDLE console = 0;
	if (!console)
	{
		console = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if (console && len > 0)
	{
		CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		GetConsoleScreenBufferInfo(console, &buffer_info);
		switch (severity)
		{
			case MsgSeverity::kError:
			case MsgSeverity::kError2:
				SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
				break;

			case MsgSeverity::kWarn:
				SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
		}
		DWORD out;
		SetConsoleTextAttribute(console, buffer_info.wAttributes);
		WriteFile(console, buf, len, &out, NULL);
	}
	else
	{
		fprintf(stream, buf);
	}
}

void Compiler::ReadScript(const char* fileName)
{
	mFileNames.push_back(fileName);

	std::ifstream inputStream;
	inputStream.open(fileName, std::ifstream::in);
	if (!inputStream.good())
	{
		Message(MsgSeverity::kError, "File %s not found\n", fileName);
		return;
	}

	mScriptSource << inputStream.rdbuf();

	Tokenizer tokenizer(&mScriptSource);
	tokenizer.Read();

	for (size_t i = 0; i < mTokenList.size(); ++i)
	{
		mTokenList[i].Print();
		Message(MsgSeverity::kInfo, " ");
	}
}

const char* Compiler::GetFileName(int fileIndex) const
{
	return mFileNames[fileIndex].c_str();
}

Identifier Compiler::AddIdentifier(const char* first, const char* last)
{
	return mIdentifiers.AddValue(first, last);
}

Token& Compiler::CreateToken(TType tokenType, int fileIndex, int lineNumber, const char* str, int len)
{
	mTokenList.push_back(Token(tokenType, fileIndex, lineNumber, str, len));
	return mTokenList.back();
}
