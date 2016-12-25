#include "stdafx.h"
#include <stdarg.h>
#include "MemoryManager.h"
#include "Compiler.h"
#include "ScriptTokenizer.h"
#include "GlueTokenizer.h"
#include "Type.h"
#include "Variable.h"
#include "SymbolTable.h"
#include "FunctionDeclaration.h"
#include "ScriptFunction.h"
#include "Expression.h"
#include "IntConstantExpression.h"
#include "FloatConstantExpression.h"
#include "StringConstantExpression.h"
#include "FunctionCallExpression.h"
#include "IdentifierExpression.h"
#include "BinaryExpression.h"
#include "AssignmentExpression.h"
#include "LogicalExpression.h"
#include "UnaryExpression.h"
#include "Statement.h"
#include "CompoundStatement.h"
#include "IfStatement.h"
#include "WhileStatement.h"
#include "ReturnStatement.h"
#include "ExpressionStatement.h"
#include "ObjectFileHelper.h"
#include "GlueGenerator.h"
#include "VariableDeclarationStatement.h"
#include "BreakStatement.h"
#include "ContinueStatement.h"
#include "Platform.h"

Compiler GCompiler;

Compiler::Compiler()
	: mErrorCount(0)
	, mCurrentTokenIndex(0)
	, mCurrentTokenType(TType::kInvalid)
	, mCurrentToken(nullptr)
{
	Reset();
}

void Compiler::Reset()
{
	GMemoryManager.Reset();

	mErrorCount = 0;
	mCurrentTokenIndex = 0;
	mCurrentTokenType = TType::kInvalid;
	mCurrentToken = nullptr;
	mTokenList.clear();
	mFileNames.clear();
	mStemNames.clear();
	mScriptFunctions.clear();
	mDeclarations.clear();
	mOrderedDeclarations.clear();
	mTypes.clear();
	mIdentifiers.Reset();

	mTokenList.reserve(kInitialTokenCount);

	RegisterBuiltInType(TType::kVoid, "void", nullptr, 1);
	RegisterBuiltInType(TType::kInt, "int", "mIntVal", sizeof(int));
	RegisterBuiltInType(TType::kFloat, "double", "mDblVal", sizeof(double));
	RegisterBuiltInType(TType::kString, "const char*", "mPtrVal", sizeof(char*));
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

void Compiler::Message(MsgSeverity severity, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	Platform::MessageArgs(GetStream(severity), format, args);
	va_end(args);
}

void Compiler::Error(int errorTokenIndex, const char* message)
{
	ShowLine(errorTokenIndex, message, MsgSeverity::kError);

	if (++mErrorCount > 100)
	{
		Message(MsgSeverity::kInfo, "Maximum error count exceeded. Aborting.\n");
		exit(EXIT_FAILURE);
	}
}

void Compiler::Error(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	Platform::MessageArgs(GetStream(MsgSeverity::kError), format, args);
	Message(MsgSeverity::kError, "\n");
	va_end(args);

	if (++mErrorCount > 100)
	{
		Message(MsgSeverity::kInfo, "Maximum error count exceeded. Aborting.\n");
		exit(EXIT_FAILURE);
	}
}

void Compiler::ReadGlueHeader(const char* fileName)
{
	AddSourceFile(fileName);

	std::ifstream inputStream;
	inputStream.open(fileName, std::ios::in);
	if (!inputStream.good())
	{
		Error("File %s not found\n", fileName);
		return;
	}

	GlueTokenizer tokenizer(&inputStream, (int)mFileNames.size() - 1);
	tokenizer.Read();
}

void Compiler::ParseGlueHeader()
{
	StartParse();

	while (mCurrentTokenType != TType::kEOF)
	{
		ParseGlueDeclaration();
	}
}

void Compiler::WriteGlueFile(const char* fileName)
{
	if (fileName && *fileName)
	{
		GlueGenerator gen;

		for (FunctionDeclaration* decl : mOrderedDeclarations)
		{
			gen.AddFunction(decl);
		}

		gen.WriteGlueFile(fileName);
	}
}

void Compiler::ReadScript(const char* fileName)
{
	AddSourceFile(fileName);

	std::ifstream inputStream;
	inputStream.open(fileName, std::ios::in);
	if (!inputStream.good())
	{
		Error("File %s not found\n", fileName);
		return;
	}

	ScriptTokenizer tokenizer(&inputStream, (int)mFileNames.size() - 1);
	tokenizer.Read();
}

void Compiler::ParseScript()
{
	StartParse();

	// create a global function to contain global-scoped initializers
	ScriptFunction* globalFunc = new ScriptFunction(mCurrentTokenIndex, true);
	globalFunc->SetName(AddIdentifier("__global__"));
	globalFunc->SetReturnType(FindType(TType::kVoid));
	CompoundStatement* globalBody = new CompoundStatement(mCurrentTokenIndex);
	globalFunc->SetStatement(globalBody);
	mScriptFunctions.push_back(globalFunc);

	while (mCurrentTokenType != TType::kEOF)
	{
		ParseScriptDeclaration();
	}
}

void Compiler::AnalyzeScript()
{
	SymbolTable symbolTable;

	for (ScriptFunction* func : mScriptFunctions)
	{
		func->ResolveTypes(symbolTable);
	}
}

void Compiler::WriteObjectFile(const char* objectFileName, const char* debugFileName)
{
	ObjectFileHelper helper;
	for (ScriptFunction* func : mScriptFunctions)
	{
		helper.AddFunction(func);
	}

	helper.WriteObjectFile(objectFileName);

	if (debugFileName && *debugFileName)
	{
		helper.WriteDebugFile(debugFileName);
	}
}

Identifier Compiler::GetFileName(int fileIndex) const
{
	return mFileNames[fileIndex];
}

Identifier Compiler::GetStemName(int fileIndex) const
{
	return mStemNames[fileIndex];
}

Identifier Compiler::AddIdentifier(const char* id)
{
	return mIdentifiers.AddValue(id, id + strlen(id));
}

Identifier Compiler::AddIdentifier(const char* first, const char* last)
{
	return mIdentifiers.AddValue(first, last);
}

Identifier Compiler::ComposeIdentifier(const char* format, ...)
{
	char buf[512];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, ARRAY_COUNT(buf), format, args);
	va_end(args);

	return AddIdentifier(buf);
}

Token& Compiler::CreateToken(TType tokenType, int fileIndex, int lineNumber)
{
	mTokenList.push_back(Token(tokenType, fileIndex, lineNumber));
	return mTokenList.back();
}

FunctionDeclaration* Compiler::FindDeclaration(Identifier name)
{
	FunctionDeclarationMap::iterator it = mDeclarations.find(name);
	return it != mDeclarations.end() ? it->second : nullptr;
}

Type* Compiler::FindType(TType tokenType)
{
	Identifier id = mIdentifiers.AddValue(Token::StringRepresentation(tokenType));
	return FindType(id);
}

Type* Compiler::FindType(Identifier name)
{
	TypeMap::iterator it = mTypes.find(name);
	return it != mTypes.end() ? it->second : nullptr;
}

void Compiler::RegisterBuiltInType(TType name, const char* nativeName, const char* cellField, int size)
{
	Identifier id = mIdentifiers.AddValue(Token::StringRepresentation(name));
	mTypes.insert(std::make_pair(id, new Type(id, nativeName, cellField, size)));
}

Type* Compiler::ParseType()
{
	Type* returnType = nullptr;
	switch (mCurrentTokenType)
	{
		case TType::kInt:
		case TType::kFloat:
		case TType::kString:
		case TType::kVoid:
			returnType = FindType(mCurrentTokenType);
			NextToken();
			break;

		default:
			break;
	}

	return returnType;
}

void Compiler::ParseFunctionParameters(FunctionDeclaration* decl)
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

		if (mCurrentTokenType == TType::kLocation)
		{
			decl->SetTakesLocation(true);
			Expect(TType::kLocation);
		}
		else
		{
			Type* varType = ParseType();
			var->SetType(varType);

			decl->AddParameter(var);
		}
		
		if (mCurrentTokenType != TType::kComma)
			break;
		
		Expect(TType::kComma);
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
	FunctionDeclaration* decl = new FunctionDeclaration(mCurrentTokenIndex);
	decl->SetName(id);
	NextToken();
	Expect(TType::kOpenParen);
	ParseFunctionParameters(decl);
	Expect(TType::kCloseParen);
	Expect(TType::kColon);
	Type* returnType = ParseType();
	decl->SetReturnType(returnType);

  	mDeclarations[id] = decl;
	mOrderedDeclarations.push_back(decl);
}

void Compiler::ParseScriptDeclaration()
{
	switch (mCurrentTokenType)
	{
		case TType::kFun:
			NextToken();
			ParseScriptFunction();
			break;
		
		case TType::kIdentifier:
			ParseScriptGlobalVariable();
			break;

		default:
			Error(mCurrentTokenIndex, "Unrecognized script declaration");
			NextToken();
			break;
	}
}

void Compiler::ParseScriptGlobalVariable()
{
	VariableDeclarationStatement* stmt = ParseScriptVariableDeclaration();
	stmt->SetGlobal(true);
	mScriptFunctions[0]->AddStatement(stmt);
}

void Compiler::ParseScriptFunction()
{
	ScriptFunction* func = new ScriptFunction(mCurrentTokenIndex, false);
	Identifier name;
	if (mCurrentTokenType != TType::kIdentifier)
	{
		Error(mCurrentTokenIndex, "function name expected");
		name = AddIdentifier("__internal__");
	}
	else
	{
		name = mCurrentToken->GetIdentifier();
		NextToken();
	}

	Expect(TType::kOpenParen);
	ParseFunctionParameters(func);
	Expect(TType::kCloseParen);

	Expect(TType::kColon);
	Type* returnType = ParseType();

	func->SetReturnType(returnType);
	func->SetName(name);

	CompoundStatement* body = ParseScriptCompoundStatement();
	func->SetStatement(body);

	mScriptFunctions.push_back(func);
	mDeclarations.insert(std::make_pair(name, func));
}

Statement* Compiler::ParseScriptStatement()
{
	Statement* retVal;
	switch (mCurrentTokenType)
	{
		case TType::kIf:
			retVal = ParseScriptIfStatement();
			break;

		case TType::kWhile:
			retVal = ParseScriptWhileStatement();
			break;

		case TType::kReturn:
			retVal = ParseScriptReturnStatement();
			break;

		case TType::kBreak:
			retVal = ParseScriptBreakStatement();
			break;

		case TType::kContinue:
			retVal = ParseScriptContinueStatement();
			break;

		case TType::kOpenBrace:
			retVal = ParseScriptCompoundStatement();
			break;

		default:
			if (Peek(TType::kColon))
			{
				retVal = ParseScriptVariableDeclaration();
			}
			else
			{
				retVal = ParseScriptExpressionStatement();
			}
			break;
	}
	
	return retVal;
}

CompoundStatement* Compiler::ParseScriptCompoundStatement()
{
	CompoundStatement* cmpStmt = new CompoundStatement(mCurrentTokenIndex);

	Expect(TType::kOpenBrace);
	while (mCurrentTokenType != TType::kCloseBrace
		&& mCurrentTokenType != TType::kEOF)
	{
		Statement* sub = ParseScriptStatement();
		cmpStmt->AddStatement(sub);
	}
	Expect(TType::kCloseBrace);

	return cmpStmt;
}

IfStatement* Compiler::ParseScriptIfStatement()
{
	IfStatement* ifStmt = new IfStatement(mCurrentTokenIndex);

	Expect(TType::kIf);
	Expression* expr = ParseScriptExpression();
	ifStmt->SetExpression(expr);
	Statement* thenClause = ParseScriptStatement();
	ifStmt->SetThenClause(thenClause);

	// 'else' optional
	if (mCurrentTokenType == TType::kElse)
	{
		NextToken();
		Statement* elseClause = ParseScriptStatement();
		ifStmt->SetElseClause(elseClause);
	}

	return ifStmt;
}

WhileStatement* Compiler::ParseScriptWhileStatement()
{
	WhileStatement* whileStmt = new WhileStatement(mCurrentTokenIndex);
	Expect(TType::kWhile);
	Expression* expr = ParseScriptExpression();
	whileStmt->SetExpression(expr);
	Statement* body = ParseScriptStatement();
	whileStmt->SetLoop(body);
	return whileStmt;
}

ReturnStatement* Compiler::ParseScriptReturnStatement()
{
	ReturnStatement* retStmt = new ReturnStatement(mCurrentTokenIndex);
	Expect(TType::kReturn);
	Expression* expr = ParseScriptExpression();
	retStmt->SetExpression(expr);
	return retStmt;
}

BreakStatement* Compiler::ParseScriptBreakStatement()
{
	BreakStatement* breakStmt = new BreakStatement(mCurrentTokenIndex);
	Expect(TType::kBreak);
	return breakStmt;
}

ContinueStatement* Compiler::ParseScriptContinueStatement()
{
	ContinueStatement* continueStmt = new ContinueStatement(mCurrentTokenIndex);
	Expect(TType::kContinue);
	return continueStmt;
}

VariableDeclarationStatement* Compiler::ParseScriptVariableDeclaration()
{
	VariableDeclarationStatement* varStmt = new VariableDeclarationStatement(mCurrentTokenIndex);

	if (mCurrentTokenType != TType::kIdentifier)
	{
		Error("variable identifier expected");
		SwallowLine();
		return varStmt;
	}

	Variable* var = new Variable(mCurrentTokenIndex);
	var->SetName(mCurrentToken->GetStringValue());
	NextToken();
	Expect(TType::kColon);

	Type* type = ParseType();
	if (!type)
	{
		Error("variable type expected");
		type = FindType(TType::kInt);
	}
	var->SetType(type);
	varStmt->SetVariable(var);

	// optional initializer
	if (mCurrentTokenType == TType::kArrow)
	{
		NextToken();
		Expression* expr = ParseScriptExpression();
		if (!expr)
		{
			Error("expression expected");
		}
		varStmt->SetExpression(expr);
	}

	return varStmt;
}

ExpressionStatement* Compiler::ParseScriptExpressionStatement()
{
	ExpressionStatement* exprStmt = new ExpressionStatement(mCurrentTokenIndex);
	Expression* expr = ParseScriptExpression();
	if (!expr)
	{
		Error(mCurrentTokenIndex, "expression expected");
		SwallowLine();
	}
	exprStmt->SetExpression(expr);
	return exprStmt;
}

Expression* Compiler::ParseScriptExpression()
{
	Expression* expr = ParseScriptExpressionWithPrecedence(0);
	return expr;
}

Expression* Compiler::ParseScriptPrimaryExpression()
{
	Expression* expr = nullptr;
	switch (mCurrentTokenType)
	{
		case TType::kMinus:
		{
			UnaryExpression* unary = new UnaryExpression(mCurrentTokenIndex);
			Expect(TType::kMinus);
			unary->SetTarget(ParseScriptPrimaryExpression());
			expr = unary;
			break;
		}

		case TType::kOpenParen:
			NextToken();
			expr = ParseScriptExpressionWithPrecedence(0);
			Expect(TType::kCloseParen);
			break;
		
		case TType::kIdentifier:
			if (Peek(TType::kOpenParen))
			{
				FunctionCallExpression* callExpr = new FunctionCallExpression(mCurrentTokenIndex);
				NextToken();
				Expect(TType::kOpenParen);
				while (mCurrentTokenType != TType::kCloseParen && mCurrentTokenType != TType::kEOF)
				{
					Expression* arg = ParseScriptExpression();
					callExpr->AddActual(arg);
					if (mCurrentTokenType == TType::kComma)
					{
						NextToken();
					}
					else
					{
						break;
					}
				}
				Expect(TType::kCloseParen);
				expr = callExpr;
			}
			else
			{
				expr = new IdentifierExpression(mCurrentTokenIndex);
				NextToken();
			}
			break;
		
		case TType::kIntConstant:
			expr = new IntConstantExpression(mCurrentTokenIndex);
			NextToken();
			break;
		
		case TType::kFloatConstant:
			expr = new FloatConstantExpression(mCurrentTokenIndex);
			NextToken();
			break;
		
		case TType::kStringConstant:
			expr = new StringConstantExpression(mCurrentTokenIndex);
			NextToken();
			break;

		case TType::kEOF:
		default:
			break;
	}
	return expr;
}

Expression* Compiler::ParseScriptExpressionWithPrecedence(int minPrecedence)
{
	Expression* expr = ParseScriptPrimaryExpression();
	while (1)
	{
		BinaryExpression* binExpr = ParseScriptBinaryOperator();
		if (!binExpr)
		{
			return expr;
		}

		int precedence = binExpr->GetPrecedence();
		if (precedence >= minPrecedence)
		{
			// consume the operator
			NextToken();

			// if we're dealing with associativity, here's where we do it

			Expression* right = ParseScriptExpressionWithPrecedence(precedence);
			binExpr->SetLeft(expr);
			binExpr->SetRight(right);
			expr = binExpr;
		}
		else
		{
			break;
		}
	}

	return expr;
}

BinaryExpression* Compiler::ParseScriptBinaryOperator()
{
	switch (mCurrentTokenType)
	{
		case TType::kPlus:
		case TType::kMinus:
		case TType::kSlash:
		case TType::kAsterisk:
		case TType::kPercent:
		case TType::kEquals:
		case TType::kLessEquals:
		case TType::kGreaterEquals:
		case TType::kLessThan:
		case TType::kGreaterThan:
		case TType::kNotEquals:
			return new BinaryExpression(mCurrentTokenIndex);

		case TType::kArrow:
			return new AssignmentExpression(mCurrentTokenIndex);

		case TType::kAnd:
		case TType::kOr:
			return new LogicalExpression(mCurrentTokenIndex);

		default:
			return nullptr;
	}
}

void Compiler::StartParse()
{
	// An unlikely case in practice, but if the file is actually empty,
	// make sure the token list has its terminator on it.
	if (mTokenList.size() <= 0)
	{
		CreateToken(TType::kEOF, 0, 0);
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

void Compiler::ShowLine(int errorTokenIndex, const char* message, MsgSeverity severity)
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
	Message(severity, "%s(%d) : %s : %s\n", mFileNames[fileIndex].GetString(), line, messageLevel[(int)severity], message);

	int tokenIndex = errorTokenIndex;
	while (tokenIndex > 0 && mTokenList[tokenIndex - 1].GetLineNumber() == line)
	{
		--tokenIndex;
	}

	for (; tokenIndex < (int)mTokenList.size(); ++tokenIndex)
	{
		const Token& tok = mTokenList[tokenIndex];
		if (tok.GetLineNumber() != line)
			break;

		bool thisIsTheOne = tokenIndex == errorTokenIndex;

		if (thisIsTheOne)
			Message(severity, "<<");
		tok.Print(severity);
		if (thisIsTheOne)
			Message(severity, ">>");

		Message(severity, " ");
	}

	Message(severity, "\n");
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
		char messageBuf[32];
		SPRINTF(messageBuf, "'%s' expected", Token::StringRepresentation(expectedType));
		Error(mCurrentTokenIndex, messageBuf);
		return false;
	}
	else
	{
		NextToken();
		return true;
	}
}

void Compiler::SwallowLine()
{
	Token& startToken = mTokenList[mCurrentTokenIndex];
	int startLine = startToken.GetLineNumber();
	NextToken();
	while (1)
	{
		Token& t = mTokenList[mCurrentTokenIndex];
		if (t.GetLineNumber() != startLine)
			break;
		TType tokenType = t.GetType();
		if (tokenType == TType::kEOF)
		{
			break;
		}
		NextToken();
	}
}

void Compiler::AddSourceFile(const char* fileName)
{
	mFileNames.push_back(AddIdentifier(fileName));
	mStemNames.push_back(AddIdentifier(Platform::FileNameStem(fileName)));
}
