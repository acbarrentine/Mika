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
class ReturnStatement;
class VariableDeclarationStatement;

class Compiler
{
protected:
	typedef std::map<Identifier, FunctionDeclaration*> FunctionDeclarationMap;
	typedef std::map<Identifier, Type*> TypeMap;

	int mErrorCount;
	int mCurrentTokenIndex;
	TType mCurrentTokenType;
	Token* mCurrentToken;
	std::vector<Token> mTokenList;
	std::vector<Identifier> mFileNames;
	std::vector<Identifier> mStemNames;
	std::vector<ScriptFunction*> mScriptFunctions;
	std::vector<FunctionDeclaration*> mOrderedDeclarations;
	FunctionDeclarationMap mDeclarations;
	TypeMap mTypes;
	StringTable mIdentifiers;
	const int kInitialTokenCount = 200000;

public:
	Compiler();

	void Reset();
	void Message(MsgSeverity severity, const char* format, ...);
	void MessageArgs(MsgSeverity severity, const char* format, va_list args);
	void Error(int errorTokenIndex, const char* message);
	void Error(const char* format, ...);

	void ReadGlueHeader(const char* fileName);
	void ParseGlueHeader();
	void WriteGlueFile(const char* fileName);

	void ReadScript(const char* fileName);
	void ParseScript();
	void AnalyzeScript();
	void WriteObjectFile(const char* objectFileName, const char* debugFileName);

	Identifier GetFileName(int fileIndex) const;
	Identifier GetStemName(int fileIndex) const;
	int GetErrorCount() const { return mErrorCount; }
	Token& GetToken(int tokenIndex) { return mTokenList[tokenIndex]; }

	Identifier AddIdentifier(const char* id);
	Identifier AddIdentifier(const char* first, const char* last);
	Identifier ComposeIdentifier(const char* format, ...);
	Token& CreateToken(TType tokenType, int fileIndex, int lineNumber);

	FunctionDeclaration* FindDeclaration(Identifier name);
	Type* FindType(TType tokenType);
	Type* FindType(Identifier name);

protected:
	void RegisterBuiltInType(TType name, const char* nativeName, const char* cellField, int size);
	Type* ParseType();
	void ParseFunctionParameters(FunctionDeclaration* decl);

	void ParseGlueDeclaration();
	void ParseGlueFunctionDeclaration();

	void ParseScriptDeclaration();
	void ParseScriptGlobalVariable();
	void ParseScriptFunction();
	Statement* ParseScriptStatement();
	CompoundStatement* ParseScriptCompoundStatement();
	IfStatement* ParseScriptIfStatement();
	WhileStatement* ParseScriptWhileStatement();
	ReturnStatement* ParseScriptReturnStatement();
	ExpressionStatement* ParseScriptExpressionStatement();
	VariableDeclarationStatement* ParseScriptVariableDeclaration();
	Expression* ParseScriptExpression();
	Expression* ParseScriptPrimaryExpression();
	Expression* ParseScriptExpressionWithPrecedence(int minPrecedence);
	BinaryExpression* ParseScriptBinaryOperator();

	void StartParse();
	void NextToken();
	void ShowLine(int errorTokenIndex, const char* message, MsgSeverity severity) ;
	bool Peek(TType expectedType);
	bool Expect(TType expectedType);
	void SwallowLine();
	void AddSourceFile(const char* fileName);
};

extern Compiler GCompiler;
