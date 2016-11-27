#include "stdafx.h"
#include "GlueGenerator.h"
#include "FunctionDeclaration.h"
#include "Compiler.h"
#include "Type.h"
#include "Variable.h"

void GlueGenerator::AddFunction(FunctionDeclaration* decl)
{
	mDeclarations.push_back(decl);
}

void GlueGenerator::WriteGlueFile(const char* glueFileName)
{
	std::ofstream outStream(glueFileName);
	if (!outStream.good())
	{
		GCompiler.Error("Unable to open %s for writing.", glueFileName);
		return;
	}

	Type* voidType = GCompiler.FindType(TType::kVoid);

	outStream << "#pragma once" << std::endl << std::endl;

	for (FunctionDeclaration* decl : mDeclarations)
	{
		const char* functionName = decl->GetName().GetString();
		outStream << "void Glue_" << functionName << "(MikaVM* vm)" << std::endl;
		outStream << "{" << std::endl;

		if (decl->GetTakesLocation())
		{
			outStream << "\tMikaVM::Location& loc = vm->GetLocation();" << std::endl;
		}

		// marshal arguments
		int numParams = decl->GetParameterCount();
		for (int i = 0; i < numParams; ++i)
		{
			Variable* param = decl->GetParameter(i);
			Type* paramType = param->GetType();
			Identifier paramName = param->GetName();
			if (paramName)
			{
				outStream << "\t"
					<< paramType->GetNativeName() << " "
					<< paramName.GetString()
					<< " = (" << paramType->GetNativeName() << ")"
					<< "vm->GetFunctionArg(" << i << ")."
					<< paramType->GetCellField()
					<< ";" << std::endl;
			}
			else
			{
				outStream << "\t"
					<< paramType->GetNativeName() << " "
					<< "param" << i + 1
					<< " = (" << paramType->GetNativeName() << ")"
					<< "vm->GetFunctionArg(" << i << ")."
					<< paramType->GetCellField()
					<< ";" << std::endl;
			}
		}

		// the actual function call
		outStream << "\t";
		Type* returnType = decl->GetReturnType();
		if (returnType != voidType)
		{
			outStream << returnType->GetNativeName() << " retval = ";
		}
		outStream << functionName << "(";
		for (int i = 0; i < numParams; ++i)
		{
			if (i != 0) outStream << ", ";
			Variable* param = decl->GetParameter(i);
			Identifier paramName = param->GetName();
			if (paramName)
				outStream << paramName.GetString();
			else
				outStream << "param" << i + 1;
		}
		if (decl->GetTakesLocation())
		{
			if (numParams > 0)
				outStream << ", ";
			outStream << "loc.Func->mName, loc.LineNumber";
		}
		outStream << ");" << std::endl;
		
		if (returnType != voidType)
		{
			outStream << "vm->SetConditionRegister(retVal);" << std::endl;
		}

		outStream << "}" << std::endl << std::endl;
	}

	outStream << "void RegisterGlueFunctions(MikaVM* vm)" << std::endl;
	outStream << "{" << std::endl;
	outStream << "\tvm->RegisterGlue({" << std::endl;
	for (FunctionDeclaration* decl : mDeclarations)
	{
		const char* name = decl->GetName().GetString();
		outStream << "\t\t{ \"" << name << "\", Glue_" << name << " }," << std::endl;
	}
	outStream << "\t});" << std::endl;
	outStream << "}" << std::endl;
}
