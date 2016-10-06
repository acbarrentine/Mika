#include "stdafx.h"
#include "Compiler.h"
#include "ScriptTokenizer.h"
#include "GlueTokenizer.h"
#include "Type.h"
#include "Variable.h"
#include "FunctionDeclaration.h"
#include <stdarg.h>


Compiler GCompiler;

Compiler::Compiler()
	: mErrorCount(0)
	, mCurrentTokenIndex(0)
	, mCurrentTokenType(TType::kInvalid)
	, mCurrentToken(nullptr)
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

	if (severity == MsgSeverity::kError && mErrorCount > 100)
	{
		Message(MsgSeverity::kInfo, "Maximum error count exceeded. Aborting.\n");
		exit(EXIT_FAILURE);
	}
}

void Compiler::ReadGlue(const char* fileName)
{
	mFileNames.push_back(fileName);

	std::ifstream inputStream;
	inputStream.open(fileName, std::ifstream::in);
	if (!inputStream.good())
	{
		Message(MsgSeverity::kError, "File %s not found\n", fileName);
		return;
	}

	GlueTokenizer tokenizer(&inputStream);
	tokenizer.Read();
}

void Compiler::ParseGlue()
{
	StartParse();

	while (mCurrentTokenType != TType::kEOF)
	{
		ParseGlueDeclaration();
		NextToken();
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

	ScriptTokenizer tokenizer(&inputStream);
	tokenizer.Read();
}

void Compiler::ParseScript()
{
	StartParse();

	while (mCurrentTokenType != TType::kEOF)
	{
		mCurrentToken->Print(MsgSeverity::kInfo);
		NextToken();
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

Type* Compiler::ParseType()
{
	switch (mCurrentTokenType)
	{
		case TType::kInt:
			NextToken();
			return nullptr;

		case TType::kFloat:
			NextToken();
			return nullptr;

		case TType::kString:
			NextToken();
			return nullptr;

		default:
			return nullptr;
	}
}

void Compiler::ParseGlueDeclaration()
{
	switch (mCurrentTokenType)
	{
		case TType::kStruct:
			break;

		case TType::kIdentifier:
			ParseGlueFunctionDeclaration();
			break;

		default:
			Error(mCurrentTokenIndex, "Glue declaration expected");
			NextToken();
			break;
	}
}

void Compiler::ParseGlueFunctionDeclaration()
{
	Identifier id = mCurrentToken->GetIdentifier();
	FunctionDeclaration* decl = new FunctionDeclaration(mCurrentTokenIndex, id);
	NextToken();
	Expect(TType::kOpenParen);
	ParseGlueFunctionParameters(decl);
	Expect(TType::kCloseParen);
	Expect(TType::kColon);
	Type* returnType = ParseType();
	decl->SetReturnType(returnType);
	Expect(TType::kEOL);
}

void Compiler::ParseGlueFunctionParameters(FunctionDeclaration* decl)
{
	while (1)
	{
		if (mCurrentTokenType == TType::kCloseParen)
			return;

		Variable* var = new Variable(mCurrentTokenIndex);
		if (Peek(TType::kColon))
		{
			Identifier name;
			if (mCurrentTokenType != TType::kIdentifier)
			{
				Error(mCurrentTokenIndex, "identifier expected");
			}
			else
			{
				var->SetName(mCurrentToken->GetIdentifier());
				NextToken();
			}

			Expect(TType::kColon);
		}

		Type* varType = ParseType();
		var->SetType(varType);

		decl->AddParameter(var);

		if (mCurrentTokenType != TType::kComma)
			break;
	}
	Expect(TType::kCloseParen);
}

void Compiler::StartParse()
{
	// An unlikely case in practice, but if the file is actually empty,
	// make sure the token list has its terminator on it.
	if (mTokenList.size() <= 0)
	{
		CreateToken(TType::kEOF, 0, 0, "", 0);
	}

	if (mCurrentTokenIndex != 0)
	{
		++mCurrentTokenIndex;
	}
	mCurrentToken = &mTokenList[mCurrentTokenIndex];
	mCurrentTokenType = mCurrentToken->GetType();
}

void Compiler::NextToken()
{
	if (mCurrentTokenType == TType::kEOF)
		return;

	++mCurrentTokenIndex;
	mCurrentToken = &mTokenList[mCurrentTokenIndex];
	mCurrentTokenType = mCurrentToken->GetType();
}

void Compiler::Error(size_t errorTokenIndex, const char* message)
{
	ShowLine(errorTokenIndex, message, MsgSeverity::kError);
}

void Compiler::ShowLine(size_t errorTokenIndex, const char* message, MsgSeverity severity)
{
	const Token& errorToken = mTokenList[errorTokenIndex];
	int fileIndex = errorToken.GetFileIndex();
	int line = errorToken.GetLineNumber();

	static const char* messageLevel[] =
	{
		"info",
		"warning",
		"error",
		"error",
	};
	Message(severity, "%s(%d) : %s : %s\n", mFileNames[fileIndex].c_str(), line, messageLevel[(int)severity], message);

	size_t tokenIndex = errorTokenIndex;
	while (tokenIndex > 0 && mTokenList[tokenIndex - 1].GetLineNumber() == line)
	{
		--tokenIndex;
	}

	while (1)
	{
		if (mTokenList[tokenIndex].GetLineNumber() != line)
			break;

		const Token& tok = mTokenList[tokenIndex++];
		tok.Print(severity);
		Message(severity, " ");
	}
}

bool Compiler::Peek(TType expectedType)
{
	if (mCurrentTokenType == kEOF)
		return false;

	return mTokenList[mCurrentTokenIndex + 1].GetType() == expectedType;
}

bool Compiler::Expect(TType expectedType)
{
	if (mCurrentTokenType != expectedType)
	{
		// todo: needs to identify the token type
		Error(mCurrentTokenIndex, "expected");
		return false;
	}
	else
	{
		NextToken();
		return true;
	}
}
