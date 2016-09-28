#include "stdafx.h"
#include "Compiler.h"
#include <stdarg.h>

Compiler GCompiler;

Compiler::Compiler()
	: mErrorCount(0)
	, mTokenizer(nullptr)
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
	std::ifstream inputStream;
	inputStream.open(fileName, std::ifstream::in);
	if (!inputStream.good())
	{
		Message(MsgSeverity::kError, "File %s not found\n", fileName);
		return;
	}

	mTokenizer = new Tokenizer(&inputStream, &std::cout);
	mTokenizer->Read(fileName);
}

Token& Compiler::CreateToken(TType tokenType, int lineNumber)
{
	mTokenList.push_back(Token(tokenType, lineNumber));
	return mTokenList.back();
}
