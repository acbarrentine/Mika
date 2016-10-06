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

class FunctionDeclaration;
class Type;

class Compiler
{
protected:
	int mErrorCount;
	size_t mCurrentTokenIndex;
	TType mCurrentTokenType;
	Token* mCurrentToken;
	std::vector<Token> mTokenList;
	std::vector<std::string> mFileNames;
	StringTable mIdentifiers;
	const int kInitialTokenCount = 200000;

public:
	Compiler();

	void Message(MsgSeverity severity, const char* format, ...);
	void MessageArgs(MsgSeverity severity, const char* format, va_list args);

	void ReadGlue(const char* fileName);
	void ParseGlue();
	void ReadScript(const char* fileName);
	void ParseScript();

	const char* GetFileName(int fileIndex) const;
	int GetErrorCount() const { return mErrorCount; }
	Identifier AddIdentifier(const char* first, const char* last);
	Token& CreateToken(TType tokenType, int fileIndex, int lineNumber, const char* str, int len);

protected:
	Type* ParseType();
	void ParseGlueDeclaration();
	void ParseGlueFunctionDeclaration();
	void ParseGlueFunctionParameters(FunctionDeclaration* decl);

	void StartParse();
	void NextToken();
	void Error(size_t errorTokenIndex, const char* message);
	void ShowLine(size_t errorTokenIndex, const char* message, MsgSeverity severity) ;
	bool Peek(TType expectedType);
	bool Expect(TType expectedType);
};

extern Compiler GCompiler;
