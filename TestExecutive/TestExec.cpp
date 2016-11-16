/////////////////////////////////////////////////////////////////////
// TestExec.cpp - Demonstrats Project 1 Requirements               //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Young Kyu Kim, Syracuse University, ykim127@syr.edu//
/////////////////////////////////////////////////////////////////////
/*

Separate Testing package to demonstrate Project Requirements
Requires reference to SemiExp and Tokenizer

Build Process
	Required Files: TestExec.cpp    SemiExp.cpp    SemiExp.h    Tokenizer.cpp    Tokenizer.h   itokcollection.h

	Use compile.bat, then run.bat to use program

Maintenance History:
====================
ver 1.0 : 09 Feb 2016
*/
#include<iostream>
#include <string>
#include <fstream>
#include "../SemiExp/itokcollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"

using namespace Scanner;

//Display Introductory message for requirements
void displayRequirement(std::string str)
{
	std::string msg = " Demonstrating Requirement ";
	msg.append(str);
	std::cout << "\n --------------------------------------\n";
	std::cout << msg << std::endl;
	std::cout << " --------------------------------------\n";
}

//Display Content of File
void displayInput(std::string fileSpec)
{
	std::ifstream MyFile(fileSpec);
	std::string text;

	while (!MyFile.eof())
	{
		getline(MyFile, text);
		std::cout << "" << text << "\n";
	}

	MyFile.close();

}

//Initialize tokenizer and attach it to its corresponding file "fileSpec", and tokenize files
void demoTokenizer(Toker& tokenizer, std::string fileSpec, std::string msg)
{
	std::ifstream in(fileSpec);
	tokenizer.attach(&in);

	std::cout << "\n ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + "<< msg << std::endl;
	std::cout << " ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::string str = "\n Input: ";
	str.append(fileSpec);
	str.append("\n --------------------");
	std::cout << str << std::endl;

	displayInput(fileSpec);

	std::cout << " Output : ";
	std::cout << "\n ------------ " << std::endl;

	do
	{
		//tokenizer.getConsumeState()->;
		std::string tok = tokenizer.getTok();
		if (tok == "\n")
			tok = "\\n";
		std::cout << "\n Token --  " << tok;
	}	while (in.good());

	std::cout << "\n << End of Tokenizer Test >>\n" << std::endl;
}

//Initialize SemiExpression, attach tokenizer to corresponding file, and generate semi expressions
void demoSemiExp(Toker& tokenizer, SemiExp& semi, std::string fileSpec, std::string msg)
{
	std::ifstream in(fileSpec);
	tokenizer.attach(&in);

	std::cout << "\n ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + " << msg << std::endl;
	std::cout << " ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::string str = "\n Input: ";
	str.append(fileSpec);
	str.append("\n --------------------");
	std::cout << str << std::endl;

	displayInput(fileSpec);

	std::cout << " Output : ";
	std::cout << "\n ------------ " << std::endl;

	while (semi.get())
	{
		semi.show();
	}

	std::cout << "\n << End of SemiExp Test >>\n" << std::endl;
}

//Demonstrate ITok's
void demoITok(Toker& tokenizer, SemiExp& semi, std::string fileSpec, std::string msg, std::string mode, std::string data)
{
	std::ifstream in(fileSpec);
	tokenizer.attach(&in);

	std::cout << "\n ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::cout << " + " << msg << std::endl;
	std::cout << " ++++++++++++++++++++++++++++++++++++++ " << std::endl;
	std::string str = "\n Input: ";
	str.append(fileSpec);
	str.append("\n --------------------");
	std::cout << str << std::endl;

	displayInput(fileSpec);

	std::cout << " Output : ";
	std::cout << "\n ------------ " << std::endl;
	if(mode == "lower")
		semi.toggleLower(true);
		
	while (semi.get())
	{
		if (mode == "find")
		{
			std::cout << "found " << data << " in index " << semi.find(data) << " of " << semi.length() << std::endl;
			break;
		}
		else
		{
			if (mode == "remove")
				semi.remove(data);
			semi.show();
		}
	}

	std::cout << "\n << End of SemiExp Test >>\n" << std::endl;
}

#define TEST_TESTEXEC
#ifdef TEST_TESTEXEC
int main(int argc, char* args[])
{
	Toker tokenizer;
	std::string fileSpec;    //TestFile

	displayRequirement("#4 and #5: Tokenizer Package");
	demoTokenizer(tokenizer, "../Tokenizer/alphanumeric.cpp", "Tokenize Alphanumeric");
	demoTokenizer(tokenizer, "../Tokenizer/punctuator.cpp", " Tokenizing punctuator and Special characters");
	demoTokenizer(tokenizer, "../Tokenizer/punctuator2.cpp", " Demonstrating setSpecialSingleChar(',') and setSpecialCharPair(\"..\")");
	tokenizer.setSSC(',');
	tokenizer.setSCP("..");
	//std::cout << "calling tokenizer.setSSC(',') and tokenizer.setSCP(\"..\") in TestExec main (check line #165~166)\n";
	demoTokenizer(tokenizer, "../Tokenizer/punctuator2.cpp", " Demonstrating setSpecialSingleChar(',') and setSpecialCharPair(\"..\")");
	demoTokenizer(tokenizer, "../Tokenizer/comments.cpp", "Tokenize Comments");
	demoTokenizer(tokenizer, "../Tokenizer/strings.cpp", "Tokenize Quoted Strings");
	demoTokenizer(tokenizer, "../Tokenizer/TokenizerTest.cpp", "Tokenizing Sample Code");

	//Demonstrating Requirement #6 #7 and #8, SemiExpressions
	displayRequirement("#6, #7, #8: SemiExpression Package");
	SemiExp semi(&tokenizer);

	if (argc != 1)
	{
		if (strcmp(args[1], "comment") == 0)           //enable or disable comments
		{
			std::cout << "disabling comments" << std::endl;
			semi.toggleComment(false);         //false means remove comment
		}
		if (strcmp(args[2], "newline") == 0)
		{
			std::cout << "disabling newlines" << std::endl;
			semi.toggleNewline(false);         //false means discard newlines
		}
	}
	demoSemiExp(tokenizer, semi, "../Tokenizer/se_semicolon.cpp", "SemiExp: SemiColon terminator");
	demoSemiExp(tokenizer, semi, "../Tokenizer/se_braces.cpp", "SemiExp: open/closed brace terminator");
	demoSemiExp(tokenizer, semi, "../Tokenizer/se_inheritance.cpp", "SemiExp: private, protected, public terminator");
	demoSemiExp(tokenizer, semi, "../Tokenizer/se_pound.cpp", "SemiExp: # terminator");
	demoSemiExp(tokenizer, semi, "../Tokenizer/ForTest.cpp", "SemiExp: Demonstrating SemiExpression of for loop");
	demoSemiExp(tokenizer, semi, "../Tokenizer/SemiExpTest.cpp", "Generating SemiExp on Sample Code");

	displayRequirement("#9: Demonstrating ITokCollection");
	std::cout << " For this part of the demo, only important interface functions will be shown." << std::endl;
	demoITok(tokenizer, semi, "../Tokenizer/itoktest.cpp", "SemiExp: Demonstrating toLower", "lower", "");
	demoITok(tokenizer, semi, "../Tokenizer/itoktest.cpp", "SemiExp: Demonstrating find and length", "find", "KIM");
	demoITok(tokenizer, semi, "../Tokenizer/itoktest.cpp", "SemiExp: Demonstrating remove(data)", "remove", "KYU");
	std::cout << " *** For remaining implementation of ITokCollection.h, please check SemiExp.cpp files for the code";
	return 0;
}
#endif