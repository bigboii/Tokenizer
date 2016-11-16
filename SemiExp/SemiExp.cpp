///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Young Kyu Kim, ykim127@syr.edu                       //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
A SemiExpression generator that gets a collection of tokens until it encounters a terminating tokens


Maintenance History:
====================
ver 3.2 : 08 Feb 2016
- Implemented Requirement # 7
- Terminates Token collection after encountering the following conditions
	;, {, }, public:, private:, protected:, #\n.
- Added feature to toggle comments on and off
- Added feature to toggle newlines on and off
- Implemented various ITokCollection functions, such as trimFront()
ver 3.1 : Original
*/

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <algorithm>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
//#include "../Tokenizer/Tokenizer.cpp"

using namespace Scanner;

/**
	SemiExp Implementation
*/
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

bool SemiExp::get(bool clear)
{
	_tokens.clear();
	while (true)
	{
		std::string token = _pToker->getTok();  		//1. Get and add token to vector
		if (token == "")
			break;

		if(enableComment != true && _pToker->getIsComment() != true)
				push_back(token);
		if(enableComment == true)          //if comment, don't add to semi expression            
			push_back(token);

		if (token == "for")      		//2. Check terminating conditions 		
		{
			terminateFor(token);
		}
		if (token == ";" || token == "{" || token == "}")
			return true;
		if (token == "#")         //# symbol followed by newline
		{
			terminatePound(token);
			return true;
		}
		if (token == "public" || token == "private" || token == "protected")
		{
			terminateColon(token);
			return true;
		}
	}
	return false;
}

//Handles SemiExpression termination of for loop
void SemiExp::terminateFor(std::string& terminator)
{
	int count = 0;        //count number of semicolons
	while (count != 2)
	{
		std::string token = _pToker->getTok();
		push_back(token);
		if (token == ";")
			count++;
	}
}

//Handles SemiExpression termination of # \n
void SemiExp::terminatePound(std::string& terminator)
{

	while (_tokens.back() != "\n")
	{
		terminator = _pToker->getTok();
		push_back(terminator);
	}
}

//Handles SemiExpression termination of public:, private:, and protected:
void SemiExp::terminateColon(std::string& terminator)
{
	while (terminator != ":")
	{
		terminator = _pToker->getTok();
		push_back(terminator);
	}
}

//[] overloading
std::string& SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

//returns token count of semi expression
size_t SemiExp::length()
{
	return _tokens.size();
}

//displays a SemiExpression with or without newlines based on boolean flag showNewLines
void SemiExp::show(bool showNewLines)
{
	if (enableNewline == false)
	{
		trimFront();
		std::cout << "\n  -- semiExpression --\n";
	}
	else
		std::cout << "\n  -- semiExpression (include newline) --\n";
	if (enableLower == true)
		toLower();

	for (auto token : _tokens)
	{
		if (enableNewline == false)
		{
			/*
			if (token != "\n")
				std::cout << token << " ";
			*/
			if (token == "\n")
				std::cout << "\\n" << " ";
			else
				std::cout << token << " ";
		}
		else
		{
			if (token == "\n")
				std::cout << "\\n" << " ";
			else
				std::cout << token << " ";
		}
	}
	std::cout << "\n";
}

/**
	ITokCollection Implementation
*/
size_t SemiExp::find(const std::string& tok)
{

	return 1;
}

//Replica of vector's pushback for semi expression
void SemiExp::push_back(const std::string& tok)
{
	_tokens.push_back(tok);
}

//removes corresponding token in semi expression
bool SemiExp::remove(const std::string& tok)
{
	//int shiftIndex;

	for (int i = 0; i < (int)_tokens.size(); i++)
	{
		if (_tokens[i] == tok)
		{
			_tokens.erase(_tokens.begin() + i);
			return true;
		}
	}

	return false;
}

//removes token based on index
bool SemiExp::remove(int i)
{
	if (i >= 0 || i < (int)_tokens.size())
	{
		_tokens.erase(_tokens.begin() + i);
		return true;
	}

	return false;
}

//lowercase all the characters, UNECCESSARY?
void SemiExp::toLower()
{
	int i = 0;
	for (int i = 0; i < (int)_tokens.size(); i++)
	{
		std::transform(_tokens[i].begin(), _tokens[i].end(), _tokens[i].begin(), tolower);
	}
}

//Trim \n lines in the begining of semi expressions
void SemiExp::trimFront()
{
	bool hasNewline = true;
	//std::cout << _tokens.size() << std::endl;   //debugging
	while (hasNewline)
	{
		hasNewline = false;
		if (_tokens.size() != 0)
		{
			if (_tokens[0] == "\n")
			{
				_tokens.erase(_tokens.begin());
				hasNewline = true;
			}
		}
	}
}

//Enable or disable comments. True = keep comments,  False = discard comments
void SemiExp::toggleComment(bool flag)
{
	enableComment = flag;
}

//Enable or disable comments. True = keep comments, False = discard comments
void SemiExp::toggleNewline(bool flag)
{
	enableNewline = flag;
}

void SemiExp::toggleLower(bool flag) 
{
	enableLower = flag;
}

//Clears semiexpression
void SemiExp::clear()
{
	_tokens.clear();
}
	
/**
	Main
*/
//#define TEST_SEMIEXPRESSION
#ifdef TEST_SEMIEXPRESSION
int main(int argc, char* args[])
{
	//Check for arguments
	/*
	if (argc > 1)
	{
		if(args[2] == "\c")           //enable or disable comments
	}
	*/

	Toker toker;
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/SemiExpTest.cpp";
	std::string fileSpec = "../Tokenizer/test4.cpp";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	//SemiExp* semis;
	semi.toggleComment(false);        //toggle comments
	while (semi.get())
	{
		//semi.show(true);    //show newlines as well
		semi.show(false);
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- (End) --";
		semi.show();
		std::cout << "End of SemiExpression Test\n\n";
	}
	return 0;
}
#endif