#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Young Kyu Kim, ykim127@syr.edu                       //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "itokcollection.h"
#include "../Tokenizer/Tokenizer.h"

namespace Scanner
{
	using Token = std::string;

	class SemiExp : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker);
		bool get(bool clear = true);
		std::string& operator[](size_t n);
		size_t length();
		void show(bool showNewLines = false);
		size_t find(const std::string& tok);
		void push_back(const std::string& tok);
		bool remove(const std::string& tok);
		bool remove(int i);
		void toLower();
		void trimFront();
		void clear();

		//Q's functions
		void toggleNewline(bool flag);
		void toggleComment(bool flag);
		void toggleLower(bool flag);

		void terminatePound(std::string& terminator);
		void terminateFor(std::string& terminator);
		void terminateColon(std::string& terminator);

	private:
		std::vector<Token> _tokens;
		Toker* _pToker;
		bool enableComment = false;
		bool enableNewline = true;

		//ITokVariables
		bool enableLower = false;

	};
}
#endif
