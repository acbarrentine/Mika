#include "stdafx.h"
#include "GlueGenerator.h"
#include "MemberFunctionDeclaration.h"
#include "Compiler.h"
#include "UserDefinedType.h"
#include "Variable.h"

void FunctionDeclaration::GenerateGlue(GlueGenerator& gen) const
{
	std::ofstream& outStream = gen.GetStream();
	Type* voidType = GCompiler.FindType(TType::kVoid);

	const char* functionName = mName.GetString();
	gen.AddGlueFunctionName(functionName);
	outStream << "void Glue_" << functionName << "(MikaVM* vm)" << std::endl;
	outStream << "{" << std::endl;

	if (mTakesLocation)
	{
		outStream << "\tMikaVM::Location& loc = vm->GetLocation();" << std::endl;
	}

	// marshal arguments
	int numParams = GetParameterCount();
	for (int i = 0; i < numParams; ++i)
	{
		Variable* param = mParameterList[i];
		Type* paramType = param->GetType();
		Identifier paramName = param->GetName();
		outStream << "\t"
			<< paramType->GetNativeName() << " "
			<< paramName.GetString()
			<< " = (" << paramType->GetNativeName() << ")"
			<< "vm->GetFunctionArg(" << i << ")."
			<< paramType->GetCellField()
			<< ";" << std::endl;
	}

	// the actual function call
	outStream << "\t";
	if (mReturnType != voidType)
	{
		outStream << mReturnType->GetNativeName() << " retval = ";
	}
	outStream << functionName << "(";
	for (int i = 0; i < numParams; ++i)
	{
		if (i != 0) outStream << ", ";
		Variable* param = mParameterList[i];
		Identifier paramName = param->GetName();
		outStream << paramName.GetString();
	}
	if (mTakesLocation)
	{
		if (numParams > 0)
			outStream << ", ";
		outStream << "loc.Func->mName, loc.LineNumber";
	}
	outStream << ");" << std::endl;

	if (mReturnType != voidType)
	{
		outStream << "\tvm->SetResultRegister(retval);" << std::endl;
	}

	outStream << "}" << std::endl << std::endl;
}

void MemberFunctionDeclaration::GenerateGlue(GlueGenerator& gen) const
{
	std::ofstream& outStream = gen.GetStream();
	Type* voidType = GCompiler.FindType(TType::kVoid);

	const char* functionName = mName.GetString();
	gen.AddGlueFunctionName(functionName);
	outStream << "void Glue_" << functionName << "(MikaVM* vm)" << std::endl;
	outStream << "{" << std::endl;

	if (mTakesLocation)
	{
		outStream << "\tMikaVM::Location& loc = vm->GetLocation();" << std::endl;
	}

	// param 0 is the object
	if (!mIsConstructor)
	{
		outStream << "\t"
			<< mParentType->GetNativeName() << " obj = ("
			<< mParentType->GetNativeName() << ")vm->GetFunctionArg(0)."
			<< mParentType->GetCellField()
			<< ";" << std::endl;
	}

	// marshal arguments
	int numParams = GetParameterCount();
	int paramIndex = mIsConstructor ? 0 : 1;

	for (int i = 0; i < numParams; ++i)
	{
		Variable* param = mParameterList[i];
		Type* paramType = param->GetType();
		Identifier paramName = param->GetName();
		outStream << "\t"
			<< paramType->GetNativeName() << " "
			<< paramName.GetString()
			<< " = (" << paramType->GetNativeName() << ")"
			<< "vm->GetFunctionArg(" << paramIndex++ << ")."
			<< paramType->GetCellField()
			<< ";" << std::endl;
	}

	// the actual function call
	outStream << "\t";
	if (mReturnType != voidType)
	{
		outStream << mReturnType->GetNativeName() << " retval = ";
	}

	if (mIsConstructor)
	{
		outStream << "new " << mParentType->GetName();
	}
	else
	{
		outStream << "obj->" << mMemberName.GetString();
	}
	outStream << "(";
	for (int i = 0; i < numParams; ++i)
	{
		if (i != 0) outStream << ", ";
		Variable* param = mParameterList[i];
		Identifier paramName = param->GetName();
		outStream << paramName.GetString();
	}
	if (mTakesLocation)
	{
		if (numParams > 0)
			outStream << ", ";
		outStream << "loc.Func->mName, loc.LineNumber";
	}
	outStream << ");" << std::endl;

	if (mReturnType != voidType)
	{
		outStream << "\tvm->SetResultRegister(retval);" << std::endl;
	}

	if (mDestroyReturnType)
	{
		outStream << "\tvm->RegisterDestructor(retval, Glue_Delete" << mParentType->GetName() << ");\n";
	}

	outStream << "}" << std::endl << std::endl;
}

void UserDefinedType::GenerateGlue(GlueGenerator& gen) const
{
	if (mHasConstructor)
	{
		std::ofstream& outStream = gen.GetStream();
		outStream << "void Glue_Delete" << mName.GetString() << "(void* addr)\n";
		outStream << "{\n";
		outStream << "\t" << mNativeName << " obj = ("
			<< mNativeName << ")addr;\n";
		outStream << "\tdelete obj;\n";
		outStream << "}\n\n";
	}

	for (const MemberFunctionDeclaration* decl : mOrderedDeclarations)
	{
		decl->GenerateGlue(gen);
	}
}

void GlueGenerator::AddFunctions(std::vector<FunctionDeclaration*>& declarations)
{
	mDeclarations.insert(mDeclarations.end(), declarations.begin(), declarations.end());
}

void GlueGenerator::AddUserDefinedTypes(std::vector<UserDefinedType*>& userDefinedTypes)
{
	mUserDefinedTypes.insert(mUserDefinedTypes.end(), userDefinedTypes.begin(), userDefinedTypes.end());
}

void GlueGenerator::WriteGlueFile(const char* glueFileName)
{
	mOutStream.open(glueFileName);
	if (!mOutStream.good())
	{
		GCompiler.Error("Unable to open %s for writing.", glueFileName);
		return;
	}

	mOutStream << "#pragma once" << std::endl << std::endl;

	for (const FunctionDeclaration* decl : mDeclarations)
	{
		decl->GenerateGlue(*this);
	}

	for (const UserDefinedType* type : mUserDefinedTypes)
	{
		type->GenerateGlue(*this);
	}

	mOutStream << "void RegisterGlueFunctions(MikaVM* vm)" << std::endl;
	mOutStream << "{" << std::endl;
	mOutStream << "\tvm->RegisterGlue({" << std::endl;
	for (const char* name : mGlueFunctions)
	{
		mOutStream << "\t\t{ \"" << name << "\", Glue_" << name << " }," << std::endl;
	}
	mOutStream << "\t});" << std::endl;
	mOutStream << "}" << std::endl;
}

void GlueGenerator::AddGlueFunctionName(const char* name)
{
	mGlueFunctions.emplace_back(name);
}
