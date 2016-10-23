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

class Type;
class ScriptFunction;
class FunctionDeclaration;
class Expression;
class BinaryExpression;
class Statement;
class CompoundStatement;
class IfStatement;
class WhileStatement;
class ExpressionStatement;

class Compiler
{
protected:
	typedef std::map<Identifier, FunctionDeclaration*> FunctionDeclarationMap;
	typedef std::map<Identifier, Type*> TypeMap;

	int mErrorCount;
	size_t mCurrentTokenIndex;
	TType mCurrentTokenType;
	Token* mCurrentToken;
	std::vector<Token> mTokenList;
	std::vector<std::string> mFileNames;
	std::vector<ScriptFunction*> mScriptFunctions;
	FunctionDeclarationMap mDeclarations;
	TypeMap mTypes;
	StringTable mIdentifiers;
	const int kInitialTokenCount = 200000;

public:
	Compiler();

	void Message(MsgSeverity severity, const char* format, ...);
	void MessageArgs(MsgSeverity severity, const char* format, va_list args);
	void Error(size_t errorTokenIndex, const char* message);
	void Error(const char* format, ...);

	void ReadGlue(const char* fileName);
	void ParseGlue();
	void ReadScript(const char* fileName);
	void ParseScript();
	void AnalyzeScript();
	void WriteObjectFile(const char* fileName);

	const char* GetFileName(int fileIndex) const;
	int GetErrorCount() const { return mErrorCount; }
	Token& GetToken(size_t tokenIndex) { return mTokenList[tokenIndex]; }

	Identifier AddIdentifier(const char* id);
	Identifier AddIdentifier(const char* first, const char* last);
	Token& CreateToken(TType tokenType, int fileIndex, int lineNumber);

	FunctionDeclaration* FindDeclaration(Identifier name);
	Type* FindType(TType tokenType);
	Type* FindType(Identifier name);

protected:
	void RegisterBuiltInType(TType name, size_t size);
	Type* ParseType();
	
	void ParseGlueDeclaration();
	void ParseGlueFunctionDeclaration();
	void ParseGlueFunctionParameters(FunctionDeclaration* decl);

	void ParseScriptDeclaration();
	void ParseScriptFunction();
	Statement* ParseScriptStatement();
	Statement* ParseScriptCompoundStatement();
	Statement* ParseScriptIfStatement();
	Statement* ParseScriptWhileStatement();
	Statement* ParseScriptReturnStatement();
	Statement* ParseScriptExpressionStatement();
	Expression* ParseScriptExpression();
	Expression* ParseScriptPrimaryExpression();
	Expression* ParseScriptExpressionWithPrecedence(int minPrecedence);
	BinaryExpression* ParseScriptBinaryOperator();

	void StartParse();
	void NextToken();
	void ShowLine(size_t errorTokenIndex, const char* message, MsgSeverity severity) ;
	bool Peek(TType expectedType);
	bool Expect(TType expectedType);
	void SwallowTokens(TType terminator);
};

extern Compiler GCompiler;
